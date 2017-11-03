#pragma once
#include <windows.h>
#include <string>
#include "HeartBeatConfig.h"

#define DAEMONAPI _declspec(dllexport) 

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	DAEMONAPI bool StartDeamon(std::string &path,LPSTR parameter, HeartBeatConfig& cfg, bool isMaster = true);
	DAEMONAPI bool StopDaemon();

#ifdef __cplusplus
}
#endif // __cplusplus