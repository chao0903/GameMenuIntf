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
	// 请求资源类型列表
	bool getResTypeList(LPVOID recvBuff, uint32_t &recvBuffLen);
	// 根据资源类别和首字母获取资源总数
	bool getResourceTotalCountByCondition(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &recvBuffLen);
	// 请求资源列表
	bool getResourceList(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &recvBuffLen);
	// 请求游戏盘映射
	bool getAllDiskInfo(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &bufflen);
	// 请求输入法路径
	bool getIMEConfigPathList(LPVOID recvBuff, uint32_t &recvBuffLen);
	// 网友想玩的游戏
	bool reportUserWantGame(LPVOID sendBuff, uint32_t sendBuffLen);
	// 上报用户游戏信息到web后台
	bool reportUserGameInfoToWebPlatform(LPVOID sendBuff, uint32_t sendBuffLen);
	// 请求服务器最新域名
	bool getDomainFromSrv(LPVOID recvBuff, uint32_t &recvBuffLen);
	bool getResouceInfoById(LPVOID sendBuff, uint32_t sendBuffLen, LPVOID recvBuff, uint32_t &recvBuffLen);
	bool getResourceCfgNum(LPVOID recvBuff, uint32_t recvBuffLen);
	bool getResRunExeInfo(LPVOID recvBuff, uint32_t &recvBuffLen);
};
