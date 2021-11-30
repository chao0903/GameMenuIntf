#pragma once
#include <string>
#include <tchar.h>
#include <memory>
#include <stdint.h>
#include <vector>
#include <mutex>
#include <map>

// ÓÎÏ·Æô¶¯·µ»Ø×´Ì¬Âë
enum STATUS_SCODE
{
	STATE_DEFAULT = -9,
	SUCCESSED = 0,
	FAILED,
	RESOURCE_NOT_EXITS,
	DISK_NOT_EXITS
};

class GameMenuCenter
{
public:
	static std::shared_ptr<GameMenuCenter>& instance();
	~GameMenuCenter();

	void initializer();
	bool exit();

	STATUS_SCODE startLockComputer();
	STATUS_SCODE startIMEClient();
	STATUS_SCODE startVoiceSetting();
	STATUS_SCODE startMouseSetting();

	STATUS_SCODE openUrlByOwnBrowser(const char* url);

	void startGame(const int resId, const char* strResName, const char* strPath, const char* account);
	void reqUpdateDomain();
	STATUS_SCODE reportUserWantToPlat(int resId);

	STATUS_SCODE getStartGameResult();

	char* getClientPcid();
	char* getClientCfg();
	char* GetAllDomain();

protected:
	GameMenuCenter();

private:
	std::wstring getImePathByWinSysType();

private:
	STATUS_SCODE m_startGameResult;
	static std::string m_clientPcId;
};

