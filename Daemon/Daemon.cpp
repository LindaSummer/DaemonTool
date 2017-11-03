// Daemon.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>
using namespace std;

struct HeartBeatConfig
{
	HeartBeatConfig() :
		ip("192.168.1.25"),
		port(8089),
		isMaster(false)
	{}
	std::string ip;
	unsigned port;
	bool isMaster;
};

// #define DAEMONAPI _declspec(dllimport) 
// 
// #ifdef __cplusplus
// extern "C" {
// #endif // __cplusplus
// 
// 	DAEMONAPI bool StartDeamon(std::string &path, LPWSTR parameter, HeartBeatConfig& cfg);
// 	DAEMONAPI bool StopDaemon();
// 
// #ifdef __cplusplus
// }
// #endif // __cplusplus

typedef bool (*StartDeamon)(std::string &path, LPSTR parameter, HeartBeatConfig& cfg, bool isMaster);
typedef bool (*StopDaemon)();

int main()
{
	char currentPath[1024] = { 0 };
	GetModuleFileName(nullptr, currentPath, 1023);
	string path(currentPath);
	path.erase(path.rfind('\\', path.length()) + 1);
	path += "DaemonTool.dll";
	auto hDll = LoadLibrary(path.c_str());
	HeartBeatConfig cfg;
	StartDeamon Start;
	StopDaemon Stop;
	Start = (StartDeamon)GetProcAddress(hDll, "StartDeamon");
	Stop = (StopDaemon)GetProcAddress(hDll, "StopDaemon");
	cout << "ip";
	cin >> cfg.ip;
	cout << endl;
	cout << "IsMaster?" << endl;
	char tem;
	cin >> tem;
	if (tem == 'y')
	{
		cfg.isMaster = true;
	}
	else
	{
		cfg.isMaster = false;
	}
	Start(string("JustForTestProgramme.exe"), NULL, cfg, cfg.isMaster);
	Stop();


    return 0;
}

