#pragma once
#include "../dllhead.h"

GAMEMENUINTF_API bool GM_InitApi();
GAMEMENUINTF_API bool GM_UinitApi();

GAMEMENUINTF_API char* GM_GetLocalResTypeList();
GAMEMENUINTF_API char* GM_GetLocalResList(int queryPageNum, int queryPageSize, int queryResType, const char* nameFirstChar);
GAMEMENUINTF_API void GM_StartGame(int resId, const char* strResName, const char* strPath,const char* account);
GAMEMENUINTF_API void GM_ReqUpdateDomain();
GAMEMENUINTF_API int GM_GetResultAfterStartGame();

GAMEMENUINTF_API int GM_StartPersonDisk();
GAMEMENUINTF_API int GM_StartLockComputer();
GAMEMENUINTF_API int GM_StartIMEClient();
GAMEMENUINTF_API int GM_StartVoiceSetting();
GAMEMENUINTF_API int GM_StartMouseSetting();

GAMEMENUINTF_API int GM_NotifyUserWantGame(int resId);

GAMEMENUINTF_API int GM_OpenUrlByOwnBrowser(const char* url);

GAMEMENUINTF_API char* GM_GetClientPcId();

GAMEMENUINTF_API char* GM_GetClientCfgInfo();
GAMEMENUINTF_API char* GM_GetAllDomain();