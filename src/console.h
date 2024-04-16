#ifndef PIKACHUGAME_CONSOLE_H
#define PIKACHUGAME_CONSOLE_H

#include <iostream>
#include <windows.h>

#ifndef PIKACHUGAME_COORD_H
#include "coord.h"
#endif

#endif

int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;

// Return Window screen size via a struct named COORD
// Docs provided in: https://learn.microsoft.com/en-us/windows/console/getconsolescreenbufferinfo
COORD getScrSize() {
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);
	const auto newScreenWidth = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
	const auto newscreenHeight = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;

	return COORD{static_cast<short>(newScreenWidth), static_cast<short>(newscreenHeight)};
}

void syncScrSize() {
	COORD scrSize = getScrSize();
	SCREEN_WIDTH = scrSize.X;
	SCREEN_HEIGHT = scrSize.Y;
}

void disableConsoleResize() {
	HWND hWnd = GetConsoleWindow();
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void disableConsoleMinimizeButton() {
	HWND hWnd = GetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hWnd, false);

	DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
}

// Clear screen without the need of using "cls" command or ansi escape code (which will surely expand the screen buffer
// causing the console to flicker)
// Modified from the accepted answer: https://stackoverflow.com/questions/34842526/update-console-without-flickering-c
// Microsoft API: https://learn.microsoft.com/en-us/windows/console/clearing-the-screen
// Other: Tali Oat - https://stackoverflow.com/questions/34842526/update-console-without-flickering-c
// https://learn.microsoft.com/en-us/windows/console/console-screen-buffers?redirectedfrom=MSDN#_win32_character_attributes

void clrScr() {
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = {0, 0};

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		abort();
	}
	csbi.dwSize = {static_cast<SHORT>(SCREEN_WIDTH), static_cast<SHORT>(SCREEN_HEIGHT)};
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default which clears out all background colour formatting.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}

void consoleInit() {

	//Force console to use UTF-8, so it won't break ansi code due to most ansi code is written using utf-8o
	//Ref: The source is I made it up, jk: https://stackoverflow.com/a/41645159/24078702
	//Docs: https://learn.microsoft.com/en-us/windows/console/setconsoleoutputcp
	SetConsoleOutputCP(CP_UTF8); //65001

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, ENABLE_EXTENDED_FLAGS | (mode & (~ENABLE_ECHO_INPUT) & (~ENABLE_QUICK_EDIT_MODE)));

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	// Enable Ansi support, giving the power to change the color world! RGB1!!!!
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

	// Resize the console font size to 16
	// Ref: https://learn.microsoft.com/en-us/windows/console/setcurrentconsolefontex
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	GetCurrentConsoleFontEx(hOut, false, &cfi);
	cfi.dwFontSize = {0, 16};
	SetCurrentConsoleFontEx(hOut, false, &cfi);

	HWND hWnd = GetConsoleWindow();

	ShowWindow(hWnd, SW_MAXIMIZE);
	// SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, nullptr);

	//system("mode con COLS=700");
	//SendMessage(GetConsoleWindow(),WM_SYSKEYDOWN,VK_RETURN,0x20000000); Full screen

	//disableConsoleResize(); //auto resize
	//DeleteMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);
//	disableConsoleMinimizeButton();

	// Hide the cursor. The blinking one, not the mouse;
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hOut, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(hOut, &cursorInfo);

//	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
//	SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	syncScrSize();
	ShowScrollBar(hWnd, SB_BOTH, false);
//	DeleteMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_MINIMIZE, MF_BYCOMMAND);
}

void moveCursorToCoord(Coord coord) {
	if (coord.x > SCREEN_WIDTH || coord.y > SCREEN_HEIGHT) return;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdout, {static_cast<SHORT>(coord.x), static_cast<SHORT>(coord.y)});
}

// Set background color using palate!
void fillConsoleBackground(COLORREF color) {
	CONSOLE_SCREEN_BUFFER_INFOEX sbInfoEx;
	sbInfoEx.cbSize = sizeof(sbInfoEx);

	HANDLE consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfoEx(consoleOut, &sbInfoEx);

	for (unsigned long & colorInfo : sbInfoEx.ColorTable) colorInfo = color;
	//sbInfoEx.dwSize = sbInfoEx.dwMaximumWindowSize;
	SetConsoleScreenBufferInfoEx(consoleOut, &sbInfoEx);
}

void flashConsoleBackgroundColor(int cntFlashes, int flashInterval_ms, COLORREF color, COLORREF color2) {
	CONSOLE_SCREEN_BUFFER_INFOEX sbInfoEx;
	sbInfoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

	HANDLE consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfoEx(consoleOut, &sbInfoEx);

	COLORREF storedBG = sbInfoEx.ColorTable[0];

	for (int i = 0; i < 2 * cntFlashes; ++i) {
		if (i % 2 == 0) {
			sbInfoEx.ColorTable[0] = color;
		} else {
			sbInfoEx.ColorTable[0] = color2;
		}
		SetConsoleScreenBufferInfoEx(consoleOut, &sbInfoEx);
		Sleep(flashInterval_ms);
	}
}


void setConsoleColors(WORD attribs) {
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFOEX cbi;
	cbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(hOutput, &cbi);
	cbi.wAttributes = attribs;
	SetConsoleScreenBufferInfoEx(hOutput, &cbi);
}
