#pragma once

#ifdef WIN32
#define GAMEMENUINTF_API extern "C" __declspec(dllexport) 

#else
#ifndef GAMEMENUINTF_API
#define GAMEMENUINTF_API extern "C" 
#endif
#endif