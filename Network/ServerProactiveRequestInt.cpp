#include "ServerProactiveRequestInt.h"
#include "WyGameServerMsgDef.h"
#include "logfile.h"

static std::shared_ptr<CServerProactiveRequestInt> _instance = nullptr;
static std::mutex _instance_mutex;
std::map<unsigned int, std::function<bool(CServerProactiveRequestInt&, const char*, int)> > CServerProactiveRequestInt::m_handleCltRespondMap = {};

std::shared_ptr<CServerProactiveRequestInt>& CServerProactiveRequestInt::instance()
{
	if (nullptr == _instance)
	{
		std::lock_guard<std::mutex> lock(_instance_mutex);
		if (nullptr == _instance) {
			_instance.reset(new CServerProactiveRequestInt);
		}
	}
	return _instance;
}


CServerProactiveRequestInt::CServerProactiveRequestInt()
{
	initHandleCltRespondMap();
}

CServerProactiveRequestInt::~CServerProactiveRequestInt()
{

}

void CServerProactiveRequestInt::initHandleCltRespondMap()
{
	m_handleCltRespondMap.insert(std::make_pair(WyGameServer::CMD_CLT_GET_GAME_DRIVER, &CServerProactiveRequestInt::setGameDiskMap));
}

void CServerProactiveRequestInt::ProcessRespond(int iCmd, const char* buf, int len)
{
	auto iter = m_handleCltRespondMap.find(iCmd);
	if (iter != m_handleCltRespondMap.end())
	{
		if (!iter->second(*this, buf, len))
		{
			g_logger->debug("[ProcessRespond] CMD {} failed.", iCmd);
		}
	}
}

bool CServerProactiveRequestInt::setGameDiskMap(const char* buf, int len)
{
	return true;
}