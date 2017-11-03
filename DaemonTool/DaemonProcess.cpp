#include "DaemonProcess.h"
#include <string>
#include <windows.h>
#include <io.h>

unsigned DaemonProcess::StartProcess(void* arg)
{
	auto args_In = reinterpret_cast<argIn *>(arg);
	path_ = args_In->path;
// 	if (_access(path_.c_str(), 0) != 0);
// 	{
// 		return false;
// 	}
	char* para = static_cast<char*>(args_In->lpParameter);
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof startupInfo);
	std::string cmd;
	if (para != nullptr)
	{
		cmd = path_ + ' ' + std::string(para);
	}
	else
	{
		cmd = path_;
	}
	
	char pCmd[10240] = { 0 };
	strcpy_s(pCmd, cmd.c_str());
	terminateFlag = false;
	while (!terminateFlag)
	{
		if (!startFlag_)
		{
			if (!CreateProcess(NULL, pCmd, NULL, NULL, false, 0, NULL, NULL, &startupInfo, &processInfo_))
			{
				return -1;
			}
			startFlag_ = true;
			WaitForSingleObject(processInfo_.hProcess, INFINITE);
		}
		else
		{
			CloseHandle(processInfo_.hProcess);
			CloseHandle(processInfo_.hThread);
			startFlag_ = false;
			if (--args_In->times == 0)
			{
				break;
			}
		}
		Sleep(5000);
	}

	return 0;
}

bool DaemonProcess::Status()
{
	return startFlag_;
}

bool DaemonProcess::StopProcess()
{
	DWORD exitcode(0);
	terminateFlag = true;
	if (startFlag_)
	{
		auto ret = TerminateProcess(processInfo_.hProcess, exitcode);
		if (ret)
		{
			startFlag_ = false;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}
