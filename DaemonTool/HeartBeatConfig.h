#pragma once
#include <string>

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