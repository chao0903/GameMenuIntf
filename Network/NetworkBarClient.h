#pragma once

#include "NetToolLib.h"
#include <string>
#include <map>
#include "GlobalInt.h"

class CNetworkClient : public CAsynSocket
{
public:
	static std::shared_ptr<CNetworkClient>& instance();
	~CNetworkClient();
protected:
	CNetworkClient();

public:
	void OnConnect(BOOL bConnected);
	BOOL ProcessRespond(const string* respond, int restype);

	//保活处理
	void StartKeepAliveTimerThread();
	bool sendKeepAliveProcess();
	bool recvKeepAliveProcess(const char* buf, int len);

	//链接状态设置与获取
	void SetConnectStatus(StatusConnect conStatus) { m_connectStatus = conStatus; }
	StatusConnect GetConnectStatus(){ return m_connectStatus; }

private:
	static unsigned int __stdcall _connectThread(void *pParam);
	static unsigned int __stdcall _keepAliveTimerThread(void *pParam);

private:
	StatusConnect	m_connectStatus;
};

