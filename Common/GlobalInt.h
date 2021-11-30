#pragma once
#include <string>
#include <tchar.h>
#include <memory>
#include <stdint.h>
#include <vector>
#include <map>
#include <mutex>
#include <intsafe.h>
#include "logfile.h"

#define SERVER_PORT 9600;

// 服务器链接状态
typedef enum STATUS_CONNECT
{
	CONNECTED=0,	//已连接
	CONNECTING,		//正在链接
	NOTCONNECT		//未链接
}StatusConnect;


class CGlobalInt
{
public:
	static std::shared_ptr<CGlobalInt>& instance();
	~CGlobalInt();
protected:
	CGlobalInt();

public:
	//服务器IP地址
	std::string getServerIp();
	//服务器端口
	int getServerPort() { return SERVER_PORT; }
	//ClientMac
	std::string getClientMac();

	DWORD getPlaceId();

private:
	std::string m_serverIp;
	std::string m_clientMac;
	DWORD m_nPlaceId;
	std::map<TCHAR, TCHAR> m_gameDriverMap;
};