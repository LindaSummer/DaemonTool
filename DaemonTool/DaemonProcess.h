#pragma once
#include <string>
#include <windows.h>
#include <atomic>

struct argIn
{
	std::string path;
	LPVOID lpParameter;
	unsigned times = 720;
};

class DaemonProcess
{
public:
	DaemonProcess() :
		startFlag_(false),
		terminateFlag(false)
	{}
	unsigned StartProcess(void*arg);
	bool Status();
	bool StopProcess();
private:
	PROCESS_INFORMATION processInfo_;
	std::atomic_bool startFlag_, terminateFlag;
	std::string path_;
};