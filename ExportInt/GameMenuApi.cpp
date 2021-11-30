#include "GameMenuApi.h"
#include "logfile.h"
#include "GameMenuCenter.h"
#include "GlobalInt.h"

GAMEMENUINTF_API bool GM_InitApi()
{
	GameMenuCenter::instance()->initializer();
	
	return true;
}
GAMEMENUINTF_API bool GM_UinitApi()
{
	return GameMenuCenter::instance()->exit();
}


GAMEMENUINTF_API void GM_StartGame(int resId, const char* strResName, const char* strPath, const char* account)
{
	GameMenuCenter::instance()->startGame(resId, strResName, strPath,account);
}

GAMEMENUINTF_API void GM_ReqUpdateDomain()
{
	GameMenuCenter::instance()->reqUpdateDomain();
}

GAMEMENUINTF_API int GM_GetResultAfterStartGame()
{
	return GameMenuCenter::instance()->getStartGameResult();
}

GAMEMENUINTF_API int GM_StartPersonDisk()
{
	return 1;
}

GAMEMENUINTF_API int GM_StartLockComputer()
{
	return (int)GameMenuCenter::instance()->startLockComputer();
}

GAMEMENUINTF_API int GM_StartIMEClient()
{
	
	return (int)GameMenuCenter::instance()->startIMEClient();
}

GAMEMENUINTF_API int GM_StartVoiceSetting()
{
	return (int)GameMenuCenter::instance()->startVoiceSetting();
}

GAMEMENUINTF_API int GM_StartMouseSetting()
{
	return (int)GameMenuCenter::instance()->startMouseSetting();
}

GAMEMENUINTF_API int GM_NotifyUserWantGame(int resId)
{
	return (int)GameMenuCenter::instance()->reportUserWantToPlat(resId);
}

GAMEMENUINTF_API int GM_OpenUrlByOwnBrowser(const char* url)
{
	return (int)GameMenuCenter::instance()->openUrlByOwnBrowser(url);
}

GAMEMENUINTF_API char* GM_GetClientPcId()   //打包之前，把pc去掉
{
	return GameMenuCenter::instance()->getClientPcid();
}

GAMEMENUINTF_API char* GM_GetClientCfgInfo()
{
	return GameMenuCenter::instance()->getClientCfg();
}

GAMEMENUINTF_API char* GM_GetAllDomain()
{
	return GameMenuCenter::instance()->GetAllDomain();
}