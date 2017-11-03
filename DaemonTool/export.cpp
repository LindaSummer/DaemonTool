#include "export.h"
#include "DaemonProcess.h"
#include "HeartBeat.h"
#include <process.h>

static DaemonProcess daemon;
static HeartBeat heartBeat;

unsigned __stdcall StartP(void* arg)
{
	return daemon.StartProcess(arg);
}

unsigned __stdcall StartH(void* arg)
{
	auto cfg = reinterpret_cast<HeartBeatConfig*>(arg);
	return heartBeat.StartHeartBeat(*cfg);
}

DAEMONAPI bool StartDeamon(std::string &path, LPSTR parameter, HeartBeatConfig& cfg, bool isMaster /*= true*/)
{
	argIn arg;
	arg.path = path;
	arg.lpParameter = parameter;
	HANDLE hPros[2] = { nullptr };
	DWORD exitCode;
	bool processStarted(false);
	
	unsigned processDaemonTimes = 4;

	if (isMaster)
	{
		hPros[1] = (HANDLE)_beginthreadex(NULL, 0, StartP, &arg, 0, NULL);
		hPros[0] = (HANDLE)_beginthreadex(NULL, 0, StartH, &cfg, 0, NULL);
		WaitForSingleObject(hPros[1], INFINITE);
		heartBeat.StopHeartBeat();
		return false;
	}
	else
	{
		hPros[0] = (HANDLE)_beginthreadex(NULL, 0, StartH, &cfg, 0, NULL);
//		WaitForSingleObject(hPros[0], 1000);
		while (processDaemonTimes != 0)
		{
			if (!heartBeat.LinkStatus())
			{
				//auto flag = heartBeat.LinkStatus();
				if (!processStarted)
				{
					hPros[1] = (HANDLE)_beginthreadex(NULL, 0, StartP, &arg, 0, NULL);
					processStarted = true;
				}
				GetExitCodeThread(hPros[1], &exitCode);
				if (exitCode != STILL_ACTIVE)
				{
					--processDaemonTimes;
					daemon.StopProcess();
					processStarted = false;
				}
				GetExitCodeThread(hPros[0], &exitCode);
				if (exitCode != STILL_ACTIVE)
				{
					hPros[0] = (HANDLE)_beginthreadex(NULL, 0, StartH, &cfg, 0, NULL);
				}
				
//				WaitForMultipleObjects(2, hPros, true, INFINITE);
// 				GetExitCodeThread(hPros[0], &exitCode);
// 				if (exitCode < 0)
// 				{
// 					return false;
// 				}
// 				GetExitCodeThread(hPros[1], &exitCode);
// 				if (exitCode < 0)
// 				{
// 					return false;
// 				}
			}
			else
			{
				if (processStarted)
				{
					daemon.StopProcess();
					DeleteObject(hPros[1]);
					processStarted = false;
				}
			}
			Sleep(5000);
		}
	}
	return true;

//	return daemon.StartProcess(path, parameter) && heartBeat.StartHeartBeat(cfg);
}

DAEMONAPI bool StopDaemon()
{
	if (!daemon.StopProcess())
	{
		return false;
	}
	if (!heartBeat.StopHeartBeat())
	{
		return false;
	}
	return true;
}
