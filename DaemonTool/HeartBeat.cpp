#include "HeartBeat.h"
#include <windows.h>
#include <winsock2.h>
#include <mstcpip.h>
#include <time.h>
#include <process.h>
#include "TestPackage.h"


#pragma comment (lib, "ws2_32.lib")

unsigned HeartBeat::sendFailTimes_ = 0;
unsigned HeartBeat::recvFailTimes_ = 0;


// unsigned __stdcall HeartBeatThread(void *cfg)
// {
// 	auto config = reinterpret_cast<HeartBeatConfig*>(cfg);
// 	auto ip = config->ip;
// 	auto port = config->port;
// 	DWORD dwError, dwReturn = 0L;
// 	tcp_keepalive sKA_Settings = { 0 }, sReturned = { 0 };
// 	sKA_Settings.onoff = 1;
// 	sKA_Settings.keepalivetime = 1000; // Keep Alive in 1 sec. 
// 	sKA_Settings.keepaliveinterval = 3000; // Resend if No-Reply 
// 
// 										   //初始化 DLL
// 	WSADATA wsaData;
// 	WSAStartup(MAKEWORD(2, 2), &wsaData);
// 	//创建套接字
// 	SOCKET socketHeart = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
// 	//绑定套接字
// 	sockaddr_in sockAddr;
// 	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
// 	sockAddr.sin_family = PF_INET;  //使用IPv4地址
// 	sockAddr.sin_addr.s_addr = inet_addr(ip.c_str());  //具体的IP地址
// 	sockAddr.sin_port = htons(port);  //端口
// 	bind(socketHeart, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
// 
// 	if (WSAIoctl(socketHeart, SIO_KEEPALIVE_VALS, &sKA_Settings,
// 		sizeof(sKA_Settings), &sReturned, sizeof(sReturned), &dwReturn,
// 		NULL, NULL) != 0)
// 	{
// 		dwError = WSAGetLastError();
// 		return dwError;
// 	}
// 	else
// 	{
// 		return 0;
// 	}
//}

// unsigned __stdcall SendHeartBeat(void *arg)
// {
// 	auto heartBeat = reinterpret_cast<HeartBeat*>(arg);
// 	TestPackage testPack;
// 	while (true)
// 	{
// 		if (heartBeat->exitFlag)
// 		{
// 			return 0;
// 		}
// 		if (send(heartBeat->sHeart_, (char *)&testPack, sizeof testPack, NULL) < 0)
// 		{
// 			if (++heartBeat->sendFailTimes_ > 3)
// 			{
// 				return -1;
// 			}
// 		}
// 		Sleep(5000);
// 	}
// 	return 0;
// }
// 
// unsigned __stdcall RecvHeartBeat(void *arg)
// {
// 	auto heartBeat = reinterpret_cast<HeartBeat*>(arg);
// 
// 	TestPackage testPack;
// 	char recvBuff[1024];
// 
// 	listen(heartBeat->sHeart_, 20);
// 	while (true)
// 	{
// 		if (heartBeat->exitFlag)
// 		{
// 			return 0;
// 		}
// 		clock_t tRecv(clock());
// 
// 		if (recv(heartBeat->sHeart_, recvBuff, 1024, NULL) < 0)
// 		{
// 			if (++heartBeat->recvFailTimes_ > 9)
// 			{
// 				return -1;
// 			}
// 		}
// 		if ((clock() - tRecv) * 1.0 / CLOCKS_PER_SEC > 10)
// 		{
// 			if (++heartBeat->recvFailTimes_ > 9)
// 			{
// 				return -1;
// 			}
// 		}
// 
// 		TestPackage *recvPack = reinterpret_cast<TestPackage*>(recvBuff);
// 		if (recvPack == nullptr || 
// 			recvPack->HEAD != testPack.HEAD ||
// 			recvPack->INFO_0 != testPack.INFO_0 ||
// 			recvPack->INFO_1 != testPack.INFO_1 ||
// 			recvPack->INFO_2 != testPack.INFO_2 ||
// 			recvPack->LENGTH != testPack.LENGTH)
// 		{
// 			if (++heartBeat->recvFailTimes_ > 9)
// 			{
// 				heartBeat->linkStatus_ = false;
// 				return -1;
// 			}
// 		}
// 		else
// 		{
// 			heartBeat->linkStatus_ = true;
// 		}
// 	}
// 	return 0;
// }

unsigned HeartBeat::StartHeartBeat(HeartBeatConfig &cfg)
{
// 	if (!IniFlag)
// 	{
// 		IniFlag = true;
//		return 0;
// 	}
//	hThread = (HANDLE)_beginthreadex(NULL, 0, HeartBeatThread, &cfg, CREATE_SUSPENDED, &threadID_);
	exitFlag = false;
	TestPackage testPack;
	memset(&addr_, 0, sizeof addr_);
	addr_.sin_family = PF_INET;
	addr_.sin_addr.s_addr = inet_addr(cfg.ip.c_str());
	addr_.sin_port = htons(cfg.port);
	char buffer[1024] = { 0 };
	if (cfg.isMaster)
	{
		addr_.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(sHeart_, (SOCKADDR*)&addr_, sizeof SOCKADDR) != 0)
		{
			return -1;
		}
		listen(sHeart_, 1);
		int len = sizeof SOCKADDR_IN;
		if ((sClientSocket = accept(sHeart_, (SOCKADDR*)&clientAddr_, &len)) <= 0)
		{
			return -1;
		}
		send(sClientSocket, (char*)&testPack, sizeof testPack, 0);
		while (true)
		{
			if (exitFlag)
			{
				return 0;
			}
			if (recv(sClientSocket, buffer, 1024, 0) <= 0)
			{
				++recvFailTimes_;
			}
			else
			{
				TestPackage *recvPack = reinterpret_cast<TestPackage*>(buffer);
				if (recvPack == nullptr ||
					recvPack->HEAD != testPack.HEAD ||
					recvPack->INFO_0 != testPack.INFO_0 ||
					recvPack->INFO_1 != testPack.INFO_1 ||
					recvPack->INFO_2 != testPack.INFO_2 ||
					recvPack->LENGTH != testPack.LENGTH)
				{
					if (++recvFailTimes_ > 10)
					{
						linkStatus_ = false;
						return -1;
					}
				}
				else
				{
					send(sClientSocket, (char*)&testPack, sizeof testPack, 0);
					linkStatus_ = true;
				}
			}
			Sleep(5000);
		}
	}
	else
	{
		if (connect(sHeart_, (SOCKADDR*)&addr_, sizeof SOCKADDR) != 0)
		{
			auto ret = WSAGetLastError();
			linkStatus_ = false;
			closesocket(sHeart_);
			sHeart_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			return 1;
		}
		else if(!linkStatus_)
		{
			linkStatus_ = true;
			sendFailTimes_ = 0;
			recvFailTimes_ = 0;
		}
		while (true)
		{
			if (exitFlag)
			{
				return 0;
			}
			if (!linkStatus_)
			{
				if (connect(sHeart_, (SOCKADDR*)&addr_, sizeof SOCKADDR) != 0)
				{
					linkStatus_ = false;
					closesocket(sHeart_);
					sHeart_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
					return 1;
				}
			}
			if (sendFailTimes_ > 3 || recvFailTimes_ > 3)
			{
				linkStatus_ = false;
				closesocket(sHeart_);
				sHeart_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
				return 1;
			}
			if (send(sHeart_, (char*)&testPack, sizeof testPack, 0) <= 0)
			{
				++sendFailTimes_;
			}
			else
			{
				if (recv(sHeart_, buffer, 1024, 0) <= 0)
				{
					++recvFailTimes_;
				}
				else
				{
					TestPackage *recvPack = reinterpret_cast<TestPackage*>(buffer);
					if (recvPack == nullptr ||
						recvPack->HEAD != testPack.HEAD ||
						recvPack->INFO_0 != testPack.INFO_0 ||
						recvPack->INFO_1 != testPack.INFO_1 ||
						recvPack->INFO_2 != testPack.INFO_2 ||
						recvPack->LENGTH != testPack.LENGTH)
					{
						++recvFailTimes_;
					}
					else
					{
						linkStatus_ = true;
					}
				}
			}

			Sleep(5000);
		}
		
	}

// 	bind(sHeart_, (SOCKADDR*)&addr_, sizeof SOCKADDR);
// 	connect(sHeart_, (SOCKADDR*)&addr_, sizeof SOCKADDR);
// 
// 	HANDLE tHeart[2];
// 	tHeart[0] = (HANDLE)_beginthreadex(NULL, 0, SendHeartBeat, this, 0, NULL);
// 	tHeart[1] = (HANDLE)_beginthreadex(NULL, 0, RecvHeartBeat, this, 0, NULL);
// 	DWORD exitCode;
// 	WaitForMultipleObjects(2, tHeart, true, INFINITE);
// 	GetExitCodeThread(tHeart[0], &exitCode);
// 	if (exitCode < 0)
// 	{
// 		return -1;
// 	}
// 	GetExitCodeThread(tHeart[1], &exitCode);
// 	if (exitCode < 0)
// 	{
// 		return -1;
// 	}
// 	return 0;
}

bool HeartBeat::StopHeartBeat()
{
	exitFlag = true;
	
// 	DWORD exitCode;
// 	GetExitCodeThread(sendThread_, &exitCode);
// 	if (exitCode < 0)
// 	{
// 		return false;
// 	}
// 	GetExitCodeThread(recvThread_, &exitCode);
// 	if (exitCode < 0)
// 	{
// 		return false;
// 	}
	return true;
}

bool HeartBeat::LinkStatus()
{
	return linkStatus_;
}
