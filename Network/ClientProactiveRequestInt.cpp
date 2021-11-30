#include "windows.h"
#include "ClientProactiveRequestInt.h"
#include "NetToolLib.h"
#include "WyGameServerMsgDef.h"

static std::shared_ptr<CResourceRequestInt> _resIns = nullptr;
static std::mutex _instance_mutex;

bool CClientProactiveRequestInt::requestSendAndRecv(int cmd, LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t& recvBuffLen)
{
	try
	{
		std::string ip = CGlobalInt::instance()->getServerIp();
		if (!ip.length())
		{
			g_logger->debug("[requestSendAndRecv] Server IP is empty.");
			return false;
		}

		string respond = "";
		bool ret = SendRequestAndRevc(ip.c_str(), CGlobalInt::instance()->getServerPort(), (const char*)sendBuff, sendBuffLen, cmd, respond);
		if (ret && cmd)
		{

			const char *temp = respond.data();
			int tempRecvLen = *(uint32_t *)temp - sizeof(uint32_t);
			if (tempRecvLen >= 0)
			{
				recvBuffLen = tempRecvLen > recvBuffLen ? recvBuffLen : tempRecvLen;  // 特别要注意实际接收到的buff长度大于bufflen
				memcpy(recvBuff, temp + sizeof(uint32_t), recvBuffLen);
			}
			else 
			{
				g_logger->debug("[requestSendAndRecv] Recv is empty.");
				return false;
			}
		}
		else
		{
			g_logger->debug("[requestSendAndRecv] SendRequestAndRevc failed.");
			return false;
		}
	}
	catch (const std::exception& e)
	{
		g_logger->debug("[requestSendAndRecv] Exception {}.", e.what());
		return false;
	}
	return true;
}
bool CClientProactiveRequestInt::requestOnlySend(int cmd, LPVOID sendBuff, uint32_t sendBuffLen)
{
	std::string ip = CGlobalInt::instance()->getServerIp();
	if (!ip.length()) 
	{
		g_logger->debug("[requestOnlySend] Server IP is empty.");
		return false;
	}
	
	try
	{
		string respond = "";
		if (!SendRequestAndRevc(ip.c_str(), CGlobalInt::instance()->getServerPort(), (const char*)sendBuff, sendBuffLen, cmd, respond))
		{
			g_logger->debug("[requestOnlySend] SendRequestAndRevc failed.");
			return false;
		}
		
	}
	catch (const std::exception& e)
	{
		g_logger->debug("[requestOnlySend] Exception {}.", e.what());
		return false;
	}

	return true;
}
bool CClientProactiveRequestInt::requestOnlyRecv(int cmd, LPVOID recvBuff, uint32_t& recvBuffLen)
{
	std::string ip = CGlobalInt::instance()->getServerIp();
	if (!ip.length()) 
	{
		g_logger->debug("[requestOnlyRecv] Server IP is empty.");
		return false;
	}

	try
	{
		string respond = "";
		bool ret = SendRequestAndRevc(ip.c_str(), CGlobalInt::instance()->getServerPort(), NULL, 0, cmd, respond);

		if (ret && cmd)
		{
			const char *temp = respond.data();
			int tempRecvLen = *(uint32_t *)temp - sizeof(uint32_t);
			if (tempRecvLen >= 0)
			{
				recvBuffLen = tempRecvLen > recvBuffLen ? recvBuffLen : tempRecvLen;  // 特别要注意实际接收到的buff长度大于bufflen
				memcpy(recvBuff, temp + sizeof(uint32_t), recvBuffLen);
			}
			else 
			{
				g_logger->debug("[requestOnlyRecv] Recv is empty.");
				return false;
			}
		}
		else
		{
			g_logger->debug("[requestOnlyRecv] SendRequestAndRevc failed.");
			return false;
		}
	}
	catch (const std::exception& e)
	{
		g_logger->debug("[requestOnlyRecv] Exception {}.", e.what());
		return false;
	}
	return true;
}


std::shared_ptr<CResourceRequestInt>& CResourceRequestInt::instance()
{
	if (nullptr == _resIns)
	{
		std::lock_guard<std::mutex> lock(_instance_mutex);
		if (nullptr == _resIns) {
			_resIns.reset(new CResourceRequestInt);
		}
	}
	return _resIns;
}
CResourceRequestInt::CResourceRequestInt()
{
}
CResourceRequestInt::~CResourceRequestInt()
{
}
bool CResourceRequestInt::getResTypeList(LPVOID recvBuff, uint32_t &recvBuffLen)
{
	return requestOnlyRecv(WyGameServer::CMD_RESOURCETYPEMGR_GET_CUSTOMRESOURCETYPELIST_WITHOUT_ICON, recvBuff, recvBuffLen);
}
bool CResourceRequestInt::getResourceTotalCountByCondition(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &recvBuffLen)
{
	return requestSendAndRecv(WyGameServer::CMD_CLT_REQUESTRESOURCENUM, sendBuff, sendBuffLen, recvBuff, recvBuffLen);
}
bool CResourceRequestInt::getResourceList(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &recvBuffLen)
{
	return requestSendAndRecv(WyGameServer::CMD_CLT_REQUESTRESOURCEPAGE, sendBuff, sendBuffLen, recvBuff, recvBuffLen);
}

bool CResourceRequestInt::getAllDiskInfo(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &bufflen)
{
	return requestSendAndRecv(WyGameServer::CMD_SERVERMGR_GET_DISK_INFO, sendBuff, sendBuffLen, recvBuff, bufflen);
}
bool CResourceRequestInt::getIMEConfigPathList(LPVOID recvBuff, uint32_t &recvBuffLen)
{
	return requestOnlyRecv(WyGameServer::CMD_CLT_GET_IME_RES_CONFIG_PATH_LIST, recvBuff, recvBuffLen);
}

bool CResourceRequestInt::reportUserWantGame(LPVOID sendBuff, uint32_t sendBuffLen)
{
	return requestOnlySend(WyGameServer::CMD_CLT_REPORT_USER_GAME_INFO, sendBuff, sendBuffLen);
}

bool CResourceRequestInt::reportUserGameInfoToWebPlatform(LPVOID sendBuff, uint32_t sendBuffLen)
{
	return requestOnlySend(WyGameServer::CMD_CLT_REPORT_USER_GAME_INFO_TO_WEB_PLATFORM, sendBuff, sendBuffLen);
}

bool CResourceRequestInt::getResouceInfoById(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &recvBuffLen)
{
	return requestSendAndRecv(WyGameServer::CMD_CLT_REQUEST_RESOURCE_BY_RESID, sendBuff, sendBuffLen, recvBuff, recvBuffLen);
}

bool CResourceRequestInt::getDomainFromSrv(LPVOID recvBuff, uint32_t &recvBuffLen)
{
	return requestOnlyRecv(WyGameServer::CMD_CLT_REQUEST_SRV_DOMAIN, recvBuff, recvBuffLen);
}

bool CResourceRequestInt::getResourceCfgNum(LPVOID recvBuff, uint32_t recvBuffLen)
{
	return requestOnlyRecv(WyGameServer::CMD_CLT_REQUEST_CONFIGED_RESOURCE_COUNT, recvBuff, recvBuffLen);
}

bool CResourceRequestInt::getResRunExeInfo(LPVOID recvBuff, uint32_t &recvBuffLen)
{
	return requestOnlyRecv(WyGameServer::CMD_CLT_REQUEST_RESOURCE_RUNEXE_INFO, recvBuff, recvBuffLen);
}