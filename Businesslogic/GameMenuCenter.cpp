#include "GameMenuCenter.h"
#include "GlobalInt.h"
#include "WyGameServerMsgDef.h"
#include "RapidJsonMgr.h"
#include "ATW.h"
#include <shellapi.h>
#include "EncryptMD.h"
#include "EncryptApi.h"
#include "RegistryConfigMgr.h"
#include "encrypExportIntf.h"

static std::shared_ptr<GameMenuCenter> _instance = nullptr;
static std::once_flag _instance_flag;
std::string GameMenuCenter::m_clientPcId = "";

std::shared_ptr<GameMenuCenter>& GameMenuCenter::instance()
{
	try
	{
		std::call_once(_instance_flag, [&]() {
			_instance.reset(new GameMenuCenter);
		});
	}
	catch (const std::exception& e)
	{
		g_logger->debug("[GameMenuCenter] Get instance exception,{}", e.what());
	}

	return _instance;
}


GameMenuCenter::GameMenuCenter()
{
	//同进程下的g_logger对象不会重复创建，放在这儿确保每次调用g_logger对象都存在
	SetupLogger(g_logger, "GameMenuIntApi", 1024 * 1024 * 10);
}

GameMenuCenter::~GameMenuCenter()
{
}

void GameMenuCenter::initializer()
{
	m_startGameResult = STATE_DEFAULT;
}

bool GameMenuCenter::exit()
{
	return true;
}

//获取dll本身句柄
HMODULE GetSelfModuleHandle()
{
	//Windows内存与结构管理结构体
	MEMORY_BASIC_INFORMATION mbi;
	//查询内存的状态
	return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
}

STATUS_SCODE GameMenuCenter::startLockComputer()
{
	STATUS_SCODE scode = STATUS_SCODE::SUCCESSED;
	char dllFullPath[MAX_PATH];
	HMODULE hModuleInstance = GetSelfModuleHandle();
	GetModuleFileNameA(hModuleInstance, dllFullPath, MAX_PATH);//获取dll本身所在路径
	string strPath = (string)dllFullPath; 
	int pos = strPath.find_last_of('\\', strPath.length());
	string path = strPath.substr(0, pos) + "\\PluginSoft\\WyLockComputer.exe";

	int ret = (int)ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
	if (ret < 32)
	{
		g_logger->debug("[GameMenuCenter] [startLockComputer] failed.");
		scode = STATUS_SCODE::FAILED;
	}
	else
	{
		g_logger->debug("[GameMenuCenter] [startLockComputer] success.");
		scode = STATUS_SCODE::SUCCESSED;
	}

	return scode;
}

STATUS_SCODE GameMenuCenter::startIMEClient()
{
	STATUS_SCODE scode = STATUS_SCODE::SUCCESSED;
	char dllFullPath[MAX_PATH];
	HMODULE hModuleInstance = GetSelfModuleHandle();
	GetModuleFileNameA(hModuleInstance, dllFullPath, MAX_PATH);//获取dll本身所在路径
	string strPath = (string)dllFullPath;
	int pos = strPath.find_last_of('\\', strPath.length());
	string path = strPath.substr(0, pos) + "\\PluginSoft\\WyIMEClient.exe";

	int ret = (int)ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
	if (ret < 32)
	{
		g_logger->debug("[GameMenuCenter] [startIMEClient] failed.");
		scode = STATUS_SCODE::FAILED;
	}
	else
	{
		g_logger->debug("[GameMenuCenter] [startIMEClient] success.");
		scode = STATUS_SCODE::SUCCESSED;
	}

	return scode;
}

STATUS_SCODE GameMenuCenter::startVoiceSetting()
{
	STATUS_SCODE scode = STATUS_SCODE::SUCCESSED;
	int ret = (int)ShellExecuteA(NULL, "open", "SndVol.exe", NULL, NULL, SW_SHOWNORMAL);
	if (ret < 32)
	{
		g_logger->debug("[GameMenuCenter] [startVoiceSetting] failed.");
		scode = STATUS_SCODE::FAILED;
	}
	else
	{
		g_logger->debug("[GameMenuCenter] [startVoiceSetting] success.");
		scode = STATUS_SCODE::SUCCESSED;
	}
	return scode;
}

STATUS_SCODE GameMenuCenter::startMouseSetting()
{
	WinExec("control main.cpl ,5", SW_SHOWMAXIMIZED);
	return STATUS_SCODE::SUCCESSED;
}

STATUS_SCODE GameMenuCenter::openUrlByOwnBrowser(const char* url)
{
	STATUS_SCODE scode = STATUS_SCODE::SUCCESSED;
	char dllFullPath[MAX_PATH]; // Full path
	HMODULE hModuleInstance = GetSelfModuleHandle();
	GetModuleFileNameA(hModuleInstance, dllFullPath, MAX_PATH);//获取dll本身所在路径
	string strPath = (string)dllFullPath;    // Get full path of the file
	int pos = strPath.find_last_of('\\', strPath.length());
	string path = strPath.substr(0, pos) + "\\WyMenuBrowser.exe";
	int ret = (int)ShellExecuteA(NULL, "open", path.c_str(), url, NULL, SW_SHOWNORMAL);//可以
	if (ret < 32)
	{
		g_logger->debug("[GameMenuCenter] [openUrlByOwnBrowser] {} failed.", url);
		scode = STATUS_SCODE::FAILED;
	}
	else
	{
		g_logger->debug("[GameMenuCenter] [openUrlByOwnBrowser] {} success.", url);
		scode = STATUS_SCODE::SUCCESSED;
	}
	return scode;
}

std::wstring GameMenuCenter::getImePathByWinSysType()
{
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = nullptr;
	BOOL bIsWow64 = FALSE;

	//判断系统位数
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_IA64)
	{
		//64 位操作系统
		bIsWow64 = TRUE;
	}

	//从服务器获取已下载的输入法路径列表
	WyGameServer::ImePathArr pathArr = { 0 };
	unsigned int recvLen = sizeof(WyGameServer::ImePathArr);
	//CResourceRequestInt::instance()->getIMEConfigPathList(&pathArr, recvLen);    //需要从前端获取
	if (pathArr.iCount == 0)
	{
		g_logger->debug("[GameMenuCenter] [getImePathByWinSysType] Ime dose not exsit, please download from console.");
	}

	std::wstring imePath = L"";
	if (bIsWow64)
	{
		//获取64位或32位输入法路径
		for (int i = 0; i < pathArr.iCount; ++i)
		{
			std::wstring tmpPath = pathArr.pathArr[i];
			size_t pos = tmpPath.find(L"64");
			if (pos == std::wstring::npos)
			{
				pos = tmpPath.find(L"32");
			}
			if (pos != std::wstring::npos)
			{
				imePath = tmpPath;
				break;
			}
		}
	}
	else
	{
		//获取32位输入法路径
		for (int i = 0; i < pathArr.iCount; ++i)
		{
			std::wstring tmpPath = pathArr.pathArr[i];
			size_t pos = tmpPath.find(L"32");
			if (pos != std::wstring::npos)
			{
				imePath = tmpPath;
				break;
			}
		}
	}
	return imePath;
}

void GameMenuCenter::reqUpdateDomain()
{
	std::thread syncFun([this] {
		//CGameUseMonitor::instance()->updateDomain();
	});

	syncFun.detach();
}

void GameMenuCenter::startGame(int resId, const char* strResName, const char* strPath, const char* account)
{
	if (account != NULL)
	{
		string strUserAccout = UTF8toA(account);
		//CReportHttpInterface::instance()->updateAccount(strUserAccout);
	}
	m_startGameResult = STATE_DEFAULT;
	std::thread startGameAsyn([this, resId, strResName, strPath]() {
		// 
		//Sleep(2000);//睡眠俩秒，，方便界面端显示进度
		g_logger->debug("[GameMenuCenter] [startGame] Start Game {}.", resId);
		string _strResName = UTF8toA(strResName);
		string _strPath = UTF8toA(strPath);
		if (resId <= 0 || _strResName.empty() || _strPath.empty())
		{
			g_logger->debug("[GameMenuCenter] [startGame] Game {} params error.", _strResName.c_str());
			m_startGameResult = FAILED;
			//return FAILED;
		}

#if defined ClientReg
		std::map<TCHAR, TCHAR> _gameDriverMap; // = CGlobalInt::instance()->getGameDriverMap();   //需要从前端传递
		auto findIter = _gameDriverMap.find(_strPath[0]);
		if (findIter != _gameDriverMap.end())
		{
			_strPath[0] = _gameDriverMap[_strPath[0]];
		}
		else
		{
			g_logger->debug("[GameMenuCenter] [startGame] Does not find Game drive {}.", _strPath[0]);
			m_startGameResult = DISK_NOT_EXITS;
			//return DISK_NOT_EXITS;
		}
#else
		std::map<TCHAR, TCHAR> _gameDriverMap = CGlobalInt::instance()->getGameDriverMap();
		auto findIter = _gameDriverMap.find(_strPath[0]);
		if (findIter == _gameDriverMap.end())
		{
			g_logger->debug("[GameMenuCenter] [startGame] Does not find Game drive {}.", _strPath[0]);
			m_startGameResult = DISK_NOT_EXITS;
			//return DISK_NOT_EXITS;
		}
#endif

		STATUS_SCODE code = SUCCESSED;
		if (std::string::npos == _strPath.find(".exe"))
		{
			_strPath += "\\WYLoader.exe";
		}
		g_logger->debug("[GameMenuCenter] [startGame] _strPath {}.", _strPath.c_str());
		if (PathFileExistsA(_strPath.c_str()))
		{
			int ret = (int)ShellExecuteA(NULL, "open", _strPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
			if (ret < 32)
			{
				g_logger->debug("[GameMenuCenter] [startGame] Open game {} failed.", _strResName.c_str());
				m_startGameResult = FAILED;
			}
			else
			{
				g_logger->debug("[GameMenuCenter] [startGame] Start game {} success.", _strResName.c_str());
				m_startGameResult = SUCCESSED;
			}
		}
		else
		{
			g_logger->debug("[GameMenuCenter] [startGame] Path {} does not exist.", _strPath.c_str());
			m_startGameResult = RESOURCE_NOT_EXITS;
		}
	});
	startGameAsyn.detach();
}

STATUS_SCODE GameMenuCenter::getStartGameResult()
{
	g_logger->debug("[GameMenuCenter] [getStartGameResult] state {}.", m_startGameResult);
	return m_startGameResult;
}

STATUS_SCODE GameMenuCenter::reportUserWantToPlat(int resId)    //需要改为前端直接上报
{
	g_logger->debug("[GameMenuCenter] [reportUserWantToPlat] gameid: {} .", resId);
	UINT64 _resId = resId;
	uint32_t len = sizeof(UINT64);
	bool ret = false;//= CResourceRequestInt::instance()->reportUserWantGame(&_resId, len);
	if (ret)
	{
		return SUCCESSED;
	}
	else
	{
		g_logger->debug("[GameMenuCenter] [reportUserWantToPlat] report userGameWant failed .");
		return FAILED;
	}
}

char* GameMenuCenter::getClientPcid()
{
	std::string mac = CGlobalInt::instance()->getClientMac();
	uint32_t placeid = RegistryConfigMgr::instance()->GetPlaceId();
	std::stringstream str_stream;
	str_stream.str("");
	str_stream << placeid;
	std::string szPlaceId = str_stream.str();

	//EncryptMD *  md5 = EncryptApi::producemd();
	m_clientPcId = md5(mac + szPlaceId);
	g_logger->debug("[GameMenuCenter] getClientPcid pcid: {}", m_clientPcId.c_str());
	return &m_clientPcId[0];
}

char* GameMenuCenter::getClientCfg()
{
	return CRapidJsonMgr::instance()->GetClientCfgJsonString();
}

char* GameMenuCenter::GetAllDomain()
{
	return CRapidJsonMgr::instance()->GetAllDomain();
}