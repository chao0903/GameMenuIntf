#include "NetworkBarClient.h"
#include "ATW.h"
#include "logfile.h"
#include "WyGameServerMsgDef.h"
#include "ServerProactiveRequestInt.h"

static std::shared_ptr<CNetworkClient> _instance = nullptr;
static std::once_flag _flag;

std::shared_ptr<CNetworkClient>& CNetworkClient::instance()
{
	try
	{
		std::call_once(_flag, [&]() {
			_instance.reset(new CNetworkClient());
			});
	}
	catch (...)
	{
		_instance = nullptr;
	}

	return _instance;
}

CNetworkClient::CNetworkClient()
	: m_connectStatus(StatusConnect::NOTCONNECT)
{
	StartKeepAliveTimerThread();
}
CNetworkClient::~CNetworkClient()
{
}

void CNetworkClient::OnConnect(BOOL bConnected)
{
	if (!bConnected)
	{
		if (m_connectStatus != StatusConnect::CONNECTING)
		{
			g_logger->debug("[OnConnect] Start connecting to server...");
			m_connectStatus = StatusConnect::NOTCONNECT;
			int connectThreadHandle = _beginthreadex(NULL, 0, _connectThread, this, 0, NULL);
			if (!connectThreadHandle)
			{
				g_logger->debug("[OnConnect] Begin connect thread failed!!!");
			}
			CloseHandle((HANDLE)connectThreadHandle);
			connectThreadHandle = 0;
		}
		else
		{
			g_logger->debug("[OnConnect] Be connecting to server...");
		}
	}
}

unsigned int __stdcall CNetworkClient::_connectThread(void *pParam)
{
	int ret = 0;

	if (pParam != NULL)
	{
		CNetworkClient *pThis = (CNetworkClient *)pParam;
		while (pThis->GetConnectStatus() != StatusConnect::CONNECTED)
		{
			pThis->SetConnectStatus(StatusConnect::CONNECTING);

			ret = pThis->SynConnect(CGlobalInt::instance()->getServerIp().c_str(), CGlobalInt::instance()->getServerPort());
			if (ret)
			{
				g_logger->debug("[CNetworkClient] Connect netbar server successed.");

				pThis->sendKeepAliveProcess();
				pThis->SetConnectStatus(StatusConnect::CONNECTED);

				CGlobalInt::instance()->setGameMapDriver();
				break;
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	return ret;
}

BOOL CNetworkClient::ProcessRespond(const string * respond, int restype)
{
	int resultCode = *(int*)respond->data();
	if (!resultCode) {
		delete respond;
		return false;
	}
	int iLen = *(int*)(respond->data() + 4);
	unsigned int iCmd = *(unsigned int*)(respond->data() + 8);

	if (restype == 0)
	{
		if (iCmd == WyGameServer::CMD_CLT_KEEPALIVE)
		{
			recvKeepAliveProcess(respond->data() + 12, respond->size() - 12);
		}
	}
	else if (restype == 2)
	{
		CServerProactiveRequestInt::instance()->ProcessRespond(iCmd, respond->data() + 12, respond->size() - 12);
	}
	return TRUE;
}

bool CNetworkClient::sendKeepAliveProcess()
{
	std::string strBuf = "";
	uint32_t unCmd = WyGameServer::CMD_CLT_KEEPALIVE;
	time_t tt = time(NULL);
	strBuf.append((char*)(&unCmd), sizeof(uint32_t));
	strBuf.append((char*)(&tt), sizeof(time_t));
	return SendPacket(strBuf.data(), strBuf.size());
}
bool CNetworkClient::recvKeepAliveProcess(const char* buf, int len)
{
	if (!buf || !len) { return false; }
	WyGameServer::CltHeartBeatResponse *pResponse = (WyGameServer::CltHeartBeatResponse *)buf;
	return true;
}
void CNetworkClient::StartKeepAliveTimerThread()
{
	int timerThreadHandle = _beginthreadex(NULL, 0, _keepAliveTimerThread, this, 0, NULL);
	if (!timerThreadHandle)
	{
		g_logger->debug("StartKeepAliveTimerThread failed!!!");
	}
	CloseHandle((HANDLE)timerThreadHandle);
	timerThreadHandle = 0;
}
unsigned int __stdcall CNetworkClient::_keepAliveTimerThread(void *pParam)
{
	if (pParam != NULL)
	{
		CNetworkClient *pThis = (CNetworkClient *)pParam;
		while (true)
		{
			if (pThis->GetConnectStatus() == StatusConnect::CONNECTED)
			{
				pThis->sendKeepAliveProcess();
			}
			
			std::this_thread::sleep_for(std::chrono::seconds(50));
		}
	}

	return 0;
}