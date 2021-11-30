#include "GlobalInt.h"
#include "ATW.h"
#include "WyGameServerMsgDef.h"
#include "RegistryConfigMgr.h"
static std::shared_ptr<CGlobalInt> _instance = nullptr;
static std::once_flag _flag;

std::shared_ptr<CGlobalInt>& CGlobalInt::instance()
{
	try
	{
		std::call_once(_flag, [&]() {
			_instance.reset(new CGlobalInt());
			});
	}
	catch (...)
	{
		_instance = nullptr;
	}

	return _instance;
}

CGlobalInt::CGlobalInt()
	: m_serverIp("")
{
	m_nPlaceId = 0;
}

CGlobalInt::~CGlobalInt()
{

}

std::string CGlobalInt::getServerIp()
{
	if (m_serverIp.empty())
	{
		m_serverIp = WtoA(RegistryConfigMgr::instance()->GetServerIp());
	}

	if (m_serverIp.empty())
	{
		g_logger->debug("[CGlobalInt] [getServerIp] Server Ip address is empty.");
	}
	return m_serverIp;
}

DWORD CGlobalInt::getPlaceId()
{
	if (m_nPlaceId == 0)
	{
		m_nPlaceId = RegistryConfigMgr::instance()->GetPlaceId();
	}
	return m_nPlaceId;
}

std::string CGlobalInt::getClientMac()
{
	if (m_clientMac.empty())
	{
		m_clientMac = RegistryConfigMgr::instance()->GetClientMac();
	}

	if (m_clientMac.empty())
	{
		g_logger->debug("[CGlobalInt] [getClientMac] Client mac is empty.");
	}
	return m_clientMac;
}