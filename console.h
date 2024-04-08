#include <windows.h>
#include <iostream>

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


void disableConsoleMinimizeButton() {
	HWND hWnd = GetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hWnd, false);

	DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
}

void hideConsoleCursor(bool visible = false){
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(handle, &cursorInfo);
	cursorInfo.bVisible = visible; // set the cursor visibility
	SetConsoleCursorInfo(handle, &cursorInfo);
}

// Clear screen without the need of using "cls" command or ansi escape code (which will surely expand the screen buffer
// causing the console to flicker.
// Modified from the accepted answer: https://stackoverflow.com/questions/34842526/update-console-without-flickering-c
// Microsoft API: https://learn.microsoft.com/en-us/windows/console/clearing-the-screen
// Other: Tali Oat - https://stackoverflow.com/questions/34842526/update-console-without-flickering-c
// https://learn.microsoft.com/en-us/windows/console/console-screen-buffers?redirectedfrom=MSDN#_win32_character_attributes

void clearScreen() {
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

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

