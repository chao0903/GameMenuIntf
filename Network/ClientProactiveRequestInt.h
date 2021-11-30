#pragma once
#include <string>
#include <memory>
#include <stdint.h>
#include <vector>
#include <mutex>
#include <minwindef.h>
#include "GlobalInt.h"

class CClientProactiveRequestInt
{
public:
	CClientProactiveRequestInt() {
	}
	~CClientProactiveRequestInt() {
	}

public:
	bool requestSendAndRecv(int cmd, LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t& recvBuffLen);
	bool requestOnlySend(int cmd, LPVOID sendBuff, uint32_t sendBuffLen);
	bool requestOnlyRecv(int cmd, LPVOID recvBuff, uint32_t& recvBuffLen);
};

class CResourceRequestInt : public CClientProactiveRequestInt
{
public:
	static std::shared_ptr<CResourceRequestInt>& instance();
	~CResourceRequestInt();
protected:
	CResourceRequestInt();

public:
	// ������Դ�����б�
	bool getResTypeList(LPVOID recvBuff, uint32_t &recvBuffLen);
	// ������Դ��������ĸ��ȡ��Դ����
	bool getResourceTotalCountByCondition(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &recvBuffLen);
	// ������Դ�б�
	bool getResourceList(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &recvBuffLen);
	// ������Ϸ��ӳ��
	bool getAllDiskInfo(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &bufflen);
	// �������뷨·��
	bool getIMEConfigPathList(LPVOID recvBuff, uint32_t &recvBuffLen);
	// �����������Ϸ
	bool reportUserWantGame(LPVOID sendBuff, uint32_t sendBuffLen);
	// �ϱ��û���Ϸ��Ϣ��web��̨
	bool reportUserGameInfoToWebPlatform(LPVOID sendBuff, uint32_t sendBuffLen);
	// �����������������
	bool getDomainFromSrv(LPVOID recvBuff, uint32_t &recvBuffLen);
	bool getResouceInfoById(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &recvBuffLen);
	bool getResourceCfgNum(LPVOID recvBuff, uint32_t recvBuffLen);
	bool getResRunExeInfo(LPVOID recvBuff, uint32_t &recvBuffLen);
};
