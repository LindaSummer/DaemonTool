#pragma once
#include <stdint.h>
#pragma pack(1)
struct TestPackage
{
	uint8_t HEAD = 0x60;
	uint8_t LENGTH = 0x05;
	uint8_t INFO_0 = 0xcc;
	uint8_t INFO_1 = 0xcc;
	uint8_t INFO_2 = 0xcc;
};