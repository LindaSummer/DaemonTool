#pragma once
#include <string>
#include <winnt.h>


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

typedef bool (*DaemonMain)(std::string &path, LPSTR parameter, HeartBeatConfig& cfg, bool isMaster);
typedef bool (*StopDaemon)();


class DaemonToolAPI
{
public:
	DaemonToolAPI() :
		hDll(nullptr)
	{}
	bool InitAPI();
	bool ReleaseAPI();
	bool Daemon(std::string &path, LPSTR parameter, HeartBeatConfig& cfg);
	StopDaemon Stop;
private:
	DaemonMain Daemonmain;
	HMODULE hDll;
};