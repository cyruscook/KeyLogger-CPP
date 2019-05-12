#include "pch.h"
#include "CProgramLogger.h"


CProgramLogger::CProgramLogger()
{
}


CProgramLogger::~CProgramLogger()
{
}


std::string CProgramLogger::getForegroundWindowTitle()
{
	// Foreground Window
	HWND window = GetForegroundWindow();

	// Window title
	wchar_t title[240];

	// Get window title and add it to the var
	GetWindowText(window, title, sizeof(title));

	// Now convert our WChar to a string
	std::wstring wtitle(title);
	std::string strtitle(wtitle.begin(), wtitle.end());
	return strtitle;
}