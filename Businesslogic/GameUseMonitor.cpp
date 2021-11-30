#pragma once
#include<windows.h>
#include "GameUseMonitor.h"
#include "GlobalInt.h"
#include "ATW.h"
#include <algorithm>
#include "CameraUIControl.h"
#include "ClientProactiveRequestInt.h"
#include "ReportHttpInterface.h"
#include "RegistryConfigMgr.h"
#include "ReportHttpInterface.h"
#define  PROCESS_MONITOR_DLL  ("scctrl32.dll")
#define  CLINET_SERVICE_DIR   ("ClientService")
static const std::string GAME_MENU_PRODUCT_ID = "cyyw1603792577629";
using namespace std;
using namespace WyGameServer;
static std::shared_ptr<CGameUseMonitor> _instance = nullptr;
static std::once_flag _instance_flag;

#pragma pack(push,4)
typedef struct TIMEFIELDS_ {
	unsigned short	Year;
	unsigned short  Month;
	unsigned short  Day;
	unsigned short  Hour;
	unsigned short  Minute;
	unsigned short  Second;
} TIMEFIELDS;

typedef struct OLS_PROCESS_EVENT_NODE_ {
	BOOLEAN			create;
	ULONG			pid;
	ULONG			ppid;
	ULONG			ppid2;
	CHAR			path[MAX_PATH];
	BOOLEAN			is64bit;
	TIMEFIELDS		evtTime;
} OLS_PROCESS_EVENT_NODE, *POLS_PROCESS_EVENT_NODE;
#pragma pack(pop)



// 加载驱动
typedef int(__stdcall* StartDumpCollect)();

// 事件回调函数 进程创建/销毁时回调它
typedef void(__stdcall* fnProcessEvtCallback)(__in POLS_PROCESS_EVENT_NODE EvtNode);

// 注册进程事件回调函数
typedef BOOLEAN(__stdcall* fnSetProcessEvtCallback)(__in fnProcessEvtCallback Callback);

// 取得当前进程列表 返回有效内容字节数
typedef ULONG(__stdcall* fnGetProcessList)(__out PVOID Buffer, __in ULONG BufferLength);

// 取得当前进程列表内进程个数
typedef ULONG(__stdcall* fnGetProcessCount)(VOID);


std::shared_ptr<CGameUseMonitor>& CGameUseMonitor::instance()
{
	try
	{
		std::call_once(_instance_flag, [&]() {
			_instance.reset(new CGameUseMonitor);
		});
	}
	catch (const std::exception& e)
	{
		g_logger->debug("[GameMenuCenter] Get instance exception,{}", e.what());
	}

	return _instance;
}


CGameUseMonitor::CGameUseMonitor()
{
}

CGameUseMonitor::~CGameUseMonitor()
{
}


void covertToSysTime(TIMEFIELDS evtTime, uint64_t& systime)
{
	struct tm time;
	time.tm_year = evtTime.Year - 1900;//tm中的年份比实际年份小1900，需要减掉
	time.tm_mon = evtTime.Month - 1;//tm中的月份从0开始，需要减1
	time.tm_mday = evtTime.Day;
	time.tm_hour = evtTime.Hour;
	time.tm_min = evtTime.Minute;
	time.tm_sec = evtTime.Second;
	systime = mktime(&time);

}

void __stdcall processEventCB(__in POLS_PROCESS_EVENT_NODE pEventNode)
{
	g_logger->debug("process is create{}, pid{}, ppid{}, path{}", pEventNode->create, pEventNode->pid, pEventNode->ppid, pEventNode->path);
	wstring strExePath = AtoW(pEventNode->path);
	processMonitorTask task;
	task.pid = pEventNode->pid;
	task.bCreate = pEventNode->create;
	snprintf(task.runExe, MAX_PATH, "%s", pEventNode->path);
	auto millisec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	task.createTime = millisec_since_epoch;
	CGameUseMonitor::instance()->addReportTask(task);
}


bool CGameUseMonitor::initializer()
{  
	//初始化上报接口
	InitInfo  cInitInfo;
	cInitInfo.account_id = m_strAccount;
	cInitInfo.domainUrl = WtoA(RegistryConfigMgr::instance()->GetPlatDomain());
	cInitInfo.ip = CGlobalInt::instance()->getServerIp();
	cInitInfo.placeId = RegistryConfigMgr::instance()->GetPlaceId();
	cInitInfo.mac = CGlobalInt::instance()->getClientMac();
	cInitInfo.productCode = GAME_MENU_PRODUCT_ID;
	cInitInfo.softVersion = WtoA(RegistryConfigMgr::instance()->GetVersion());
	cInitInfo.pcId = WtoA(RegistryConfigMgr::instance()->GetPcid());
	cInitInfo.domainRegKey = WtoA(CLINET_DOMAINS_REG_PATH);
	CReportHttpInterface::instance()->initHttpInterface(cInitInfo,SERVICE_GAME_MENU);
	CReportHttpInterface::instance()->updateDomainUrl(cInitInfo.domainUrl);
	TCHAR proceePath[MAX_PATH] = { 0 };
	get_current_programme_path(proceePath);
	wstring strCurrentDir = proceePath;
	int nPos = strCurrentDir.find_last_of('\\');
	if (nPos != std::string::npos)
	{
		strCurrentDir = strCurrentDir.substr(0, nPos + 1);
	}
	string processMonDllPath = WtoA(strCurrentDir) + CLINET_SERVICE_DIR + "\\" + PROCESS_MONITOR_DLL;
	HMODULE hDll = LoadLibraryA(processMonDllPath.c_str());
	g_logger->debug("[CGameUseMonitor][initializer] load library path {}", processMonDllPath.c_str());
	if (hDll == NULL)
	{
		g_logger->debug("Wytxclientmondll.dll Not Found,dll path {}",processMonDllPath.c_str());
		return false;
	}

	fnSetProcessEvtCallback lpSetProcessEvtCallback = (fnSetProcessEvtCallback)GetProcAddress(hDll, "SetProcessEvtCallback");
	if (NULL == lpSetProcessEvtCallback)
	{
		g_logger->debug("get function pointer failed");
		return false;
	}
	bool bok = lpSetProcessEvtCallback(processEventCB);
	if (bok)
	{
		g_logger->debug("set process event callback success");
	}
	else
	{
		g_logger->debug("set process event callback failed");
	}

	std::thread loadResThread(&CGameUseMonitor::procReportAsync, this);
	loadResThread.detach();
	return bok;
}

BOOL CGameUseMonitor::get_current_programme_path(PTCHAR strPath)
{
	BOOL bResult = FALSE;

	PTCHAR strSep;

	strPath[0] = _T('\0');
	GetModuleFileName(NULL, strPath, MAX_PATH);
	strPath[MAX_PATH - 1] = _T('\0');

	strSep = _tcsrchr(strPath, _T('\\'));
	if (NULL != strSep)
	{
		strSep[0] = _T('\0');

		bResult = TRUE;
	}

	return bResult;
}

void CGameUseMonitor::addReportTask(processMonitorTask& task)
{
	m_cProcessMonitorTask.put(task);
}

void CGameUseMonitor::updateDomain()
{	
	CReportHttpInterface::instance()->setServiceType(SERVICE_GAME_MENU);
	CReportHttpInterface::instance()->reqAndUpdateDomain(CLINET_DOMAINS_REG_PATH);
}

bool CGameUseMonitor::isGameDriver(TCHAR driver)
{
	bool bResult = false;
	for (auto iter : m_cGameDriverMap)
	{
		if (iter.second == driver)
		{
			bResult = true;
			break;
		}
	}
	return bResult;
}

bool CGameUseMonitor::getResInfoByRunExe(string strRunExe, WyClientGetResSimpleInfo& runInfo)
{
	bool bFind = false;
	m_cMapResRunExe.Lock();
	for (auto iter : m_cMapResRunExe.map)
	{
		WyClientGetResSimpleInfo resRunInfo = iter.second;
		if (WtoA(resRunInfo.runExe)== strRunExe)
		{
			bFind = true;
			memcpy(&runInfo, &resRunInfo, sizeof(WyClientGetResSimpleInfo));
			break;
		}
	}
	m_cMapResRunExe.UnLock();

	if (!bFind)
	{
		loadAllResouce();
		m_cMapResRunExe.Lock();
		for (auto iter : m_cMapResRunExe.map)
		{
			WyClientGetResSimpleInfo resRunInfo = iter.second;
			if (WtoA(resRunInfo.runExe) == strRunExe)
			{
				bFind = true;
				memcpy(&runInfo, &resRunInfo, sizeof(WyClientGetResSimpleInfo));
				break;
			}
		}
		m_cMapResRunExe.UnLock();
	}
	return bFind;
}


void CGameUseMonitor::loadAllResouce()
{
	WyGameServer::WyClientGetResSimpleInfo *ptrResRunExeInfo = NULL;
	int nTotalCfgRes = 0;
	do 
	{
		if (!CResourceRequestInt::instance()->getResourceCfgNum(&nTotalCfgRes, sizeof(int)))
		{
			g_logger->debug("[CGameUseMonitor][loadAllResouce]get res {} get total config res num failed");
			break;
		}
		ptrResRunExeInfo = new WyGameServer::WyClientGetResSimpleInfo[nTotalCfgRes];
		if (NULL == ptrResRunExeInfo)
		{
			g_logger->debug("[CGameUseMonitor][loadAllResouce] membory over flow");
			break;
		}

		uint32_t bufflen = nTotalCfgRes * sizeof(WyGameServer::WyClientGetResSimpleInfo);
		if (!CResourceRequestInt::instance()->getResRunExeInfo(ptrResRunExeInfo, bufflen))
		{
			g_logger->debug("[CGameUseMonitor][loadAllResouce] getResRunExeInfo failed");
			break;
		}

		m_cMapResRunExe.Lock();
		for (int i = 0; i < nTotalCfgRes; i++)
		{
			UINT64 resId = ptrResRunExeInfo[i].resId;
			TCHAR  resRunExe[MAX_PATH] = { 0 };
			std::map<TCHAR, TCHAR>::iterator iter = m_cGameDriverMap.find(ptrResRunExeInfo[i].runExe[0]);
			if (iter == m_cGameDriverMap.end())
			{
				continue;
			}
			else
			{
				ptrResRunExeInfo[i].runExe[0] = iter->second;
				m_cMapResRunExe.map.insert(make_pair(resId, ptrResRunExeInfo[i]));
			}
		}
		m_cMapResRunExe.UnLock();

	} while (0);

	if (NULL != ptrResRunExeInfo)
	{
		delete[] ptrResRunExeInfo;
		ptrResRunExeInfo = NULL;
	}
	return;
}

void CGameUseMonitor::procReportAsync()
{
	while (true)
	{
		if (m_cGameDriverMap.size() <= 0)
		{
			m_cGameDriverMap = CGlobalInt::instance()->getGameDriverMap();
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			continue;
		}
		std::vector<processMonitorTask> vecTask = m_cProcessMonitorTask.takeAll();
		for (auto task : vecTask)
		{
			wstring strPath = AtoW(task.runExe);
			wstring runExe = strPath;
			int npos = runExe.find_last_of('\\');
			if (npos != std::wstring::npos)
			{
				runExe = runExe.substr(npos+1, runExe.length());
				if (runExe == L"WYLoader.exe")
				{
					continue;
				}
			}
	
			if (!isGameDriver(strPath[0]))
			{
				continue;
			}
			bool bCreateProcess = task.bCreate;
			if (bCreateProcess)
			{
				WyClientGetResSimpleInfo resSimpleInfo;
				if (getResInfoByRunExe(task.runExe, resSimpleInfo))
				{
					processRunInfo procInfo;
					procInfo.resId = resSimpleInfo.resId;
					procInfo.start_time = task.createTime;
					m_cMapMonitorGame.insert(make_pair(task.pid, procInfo));
					CReportHttpInterface::instance()->reportStartGameInfo("", "", resSimpleInfo.resId, WtoA(resSimpleInfo.resName), resSimpleInfo.resVersionId);
					g_logger->debug("[CGameUseMonitor][procReportAsync]  pid {} process path {} intsert moniotr map ", task.pid, task.runExe);
				}
				else
				{
					g_logger->debug("[CGameUseMonitor][procReportAsync]  pid {} process path {} not find in the resouce map",task.pid, task.runExe);
				}
			}
			else
			{
				std::map<int, processRunInfo>::iterator iter = m_cMapMonitorGame.find(task.pid);
				if (iter != m_cMapMonitorGame.end())
				{
					processRunInfo runInfo = iter->second;
					runInfo.end_time = task.createTime;

					WyClientGetResSimpleInfo resSimpleInfo;
					if (getResInfoByRunExe(task.runExe, resSimpleInfo))
					{
						uint64_t play_time = (runInfo.end_time - runInfo.start_time)/1000;
						CReportHttpInterface::instance()->reportSopGameInfo(resSimpleInfo.resId, WtoA(resSimpleInfo.resName), resSimpleInfo.resVersionId, play_time);
						m_cMapMonitorGame.erase(iter);
						g_logger->debug("[CGameUseMonitor][procReportAsync]  pid {} process path {} erase from moniotr map ", task.pid, task.runExe);
					}
					else
					{
						g_logger->debug("[CGameUseMonitor][procReportAsync]  pid {} process path {} not find in the resouce map", task.pid, task.runExe);
					}
				}
				else
				{
					g_logger->debug("[CGameUseMonitor][procReportAsync] pid {} process path {} has no start record",task.pid, task.runExe);
				}
			}

		}
	}
}

void CGameUseMonitor::setAccount(string account)
{
	if (account != m_strAccount)
	{
		m_strAccount = account;
	}
}

bool CGameUseMonitor::isGameOver(UINT64 resId)
{
	std::lock_guard<std::mutex> lock(m_cLock);
	bool bFind = true;
	for (auto moitorRes : m_cMapMonitorGame)
	{
		processRunInfo  runInfo = moitorRes.second;
		if (runInfo.resId == resId)
		{
			bFind = false;
			break;
		}
	}
	return bFind;
}