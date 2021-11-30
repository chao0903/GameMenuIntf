#pragma once
#include <string>
#include <memory>
#include <stdint.h>
#include <vector>
#include <map>
#include <mutex>

class CServerProactiveRequestInt
{
public:
	static std::shared_ptr<CServerProactiveRequestInt>& instance();
	~CServerProactiveRequestInt();
protected:
	CServerProactiveRequestInt();

public:
	void ProcessRespond(int iCmd, const char* buf, int len);

private:
	void initHandleCltRespondMap();

	//在下面添加服务器主动请求时，游戏菜单的处理函数
	bool setGameDiskMap(const char* buf, int len);

private:
	static std::map<unsigned int, std::function<bool(CServerProactiveRequestInt&, const char*, int)> > m_handleCltRespondMap;
};