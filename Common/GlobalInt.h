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

// ����������״̬
typedef enum STATUS_CONNECT
{
	CONNECTED=0,	//������
	CONNECTING,		//��������
	NOTCONNECT		//δ����
}StatusConnect;


class CGlobalInt
{
public:
	static std::shared_ptr<CGlobalInt>& instance();
	~CGlobalInt();
protected:
	CGlobalInt();

public:
	//������IP��ַ
	std::string getServerIp();
	//�������˿�
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