#include "stdafx.h"
#include "DaemonToolAPI.h"

using std::string;

bool DaemonToolAPI::InitAPI()
{
	char currentPath[1024] = { 0 };
	GetModuleFileName(nullptr, currentPath, 1023);
	string path(currentPath);
	path.erase(path.rfind('\\', path.length()) + 1);
	path += "DaemonTool.dll";
	hDll = LoadLibrary(path.c_str());

	if (hDll == nullptr)
	{
		return false;
	}
	Daemonmain = (DaemonMain)GetProcAddress(hDll, "StartDeamon");
	Stop = (StopDaemon)GetProcAddress(hDll, "StopDaemon");

	if (Daemonmain == nullptr || Stop == nullptr)
	{
		return false;
	}

	return true;
}

bool DaemonToolAPI::ReleaseAPI()
{
	if (!FreeLibrary(hDll))
	{
		return false;
	}
	hDll = nullptr;
	return true;
}

bool DaemonToolAPI::Daemon(std::string &path, LPSTR parameter, HeartBeatConfig& cfg)
{
	if (Daemonmain == nullptr)
	{
		return false;
	}
	return Daemonmain(path, parameter, cfg, cfg.isMaster);
}
