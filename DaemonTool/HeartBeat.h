#pragma once
#include <string>
#include <winsock2.h>
#include <windows.h>
#include "HeartBeatConfig.h"

class HeartBeat
{
public:
	HeartBeat() :
		sendThread_(0),
		recvThread_(0),
		exitFlag(false),
		linkStatus_(true)
		//IniFlag(false)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		sHeart_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		sClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		int flag = 1;
		setsockopt(sHeart_, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag));
		setsockopt(sHeart_, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
		setsockopt(sClientSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag));
		setsockopt(sClientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

	}
	unsigned StartHeartBeat(HeartBeatConfig &cfg);
	bool StopHeartBeat();
	bool LinkStatus();

private:
	friend unsigned __stdcall SendHeartBeat(void*);
	friend unsigned __stdcall RecvHeartBeat(void*);
	HANDLE sendThread_, recvThread_;
	static unsigned sendFailTimes_;
	static unsigned recvFailTimes_;
	SOCKET sHeart_, sClientSocket;
	sockaddr_in addr_, clientAddr_;
	bool exitFlag;
	bool linkStatus_;
	//bool IniFlag;
};