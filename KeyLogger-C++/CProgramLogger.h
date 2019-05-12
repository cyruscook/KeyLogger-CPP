#pragma once

#include <windows.h>
#include <string>


// Class for Packet Sniffing, thanks https://www.binarytides.com/packet-sniffer-code-in-c-using-winsock/
class CProgramLogger
{
public:
	CProgramLogger();
	~CProgramLogger();

	std::string getForegroundWindowTitle();
};

