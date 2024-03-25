#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cert-msc50-cpp"

#include "main.h"

using namespace std;

//
//void cls1(HANDLE hConsole) {
//	CONSOLE_SCREEN_BUFFER_INFO csbi;
//	SMALL_RECT scrollRect;
//	COORD scrollTarget;
//	CHAR_INFO fill;
//
//	// Get the number of character cells in the current buffer.
//	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
//		return;
//	}
//
//	// Scroll the rectangle of the entire buffer.
//	scrollRect.Left = 0;
//	scrollRect.Top = 0;
//	scrollRect.Right = csbi.dwSize.X;
//	scrollRect.Bottom = csbi.dwSize.Y;
//
//	// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
//	scrollTarget.X = 0;
//	scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);
//
//	// Fill with empty spaces with the buffer's default text attribute.
//	fill.Char.UnicodeChar = TEXT(' ');
//	fill.Attributes = csbi.wAttributes;
//
//	// Do the scroll
//	ScrollConsoleScreenBuffer(hConsole, &scrollRect, nullptr, scrollTarget, &fill);
//
//	// Move the cursor to the top left corner too.
//	csbi.dwCursorPosition.X = 0;
//	csbi.dwCursorPosition.Y = 0;
//
//	SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
//}
//
//
//void cls2(HANDLE hConsole) {
//	COORD coordScreen = {0, 0};    // home for the cursor
//	DWORD cCharsWritten;
//	CONSOLE_SCREEN_BUFFER_INFO csbi;
//	DWORD dwConSize;
//
//	// Get the number of character cells in the current buffer.
//
//	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
//		return;
//	}
//
//	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
//
//	// Fill the entire screen with blanks.
//
//	if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
//									(TCHAR) ' ',     // Character to write to the buffer
//									dwConSize,       // Number of cells to write
//									coordScreen,     // Coordinates of first cell
//									&cCharsWritten))// Receive number of characters written
//	{
//		return;
//	}
//
//	// Get the current text attribute.
//
//	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
//		return;
//	}
//
//	// Set the buffer's attributes accordingly.
//
//	if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
//									csbi.wAttributes, // Character attributes to use
//									dwConSize,        // Number of cells to set attribute
//									coordScreen,      // Coordinates of first cell
//									&cCharsWritten)) // Receive number of characters written
//	{
//		return;
//	}
//
//	// Put the cursor at its home coordinates.
//
//	SetConsoleCursorPosition(hConsole, coordScreen);
//}
//
//
//
//int main1() {
//	EnableAnsi();
//	int i = 0;
//
//	HANDLE hStdout;
//	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
//	while (true) {
//		printf("\x1b[2J");
//		cout << "Hello!\n" << i;
//		Sleep(1000 / 30);
//		printf("\x1b[H");
////		system("cls");
////		cls1(hStdout);
////		cout << "\033[2J\033[;H";
//
//		i++;
//		if (i == 10000) system("pause");
//	}
//
////	clrscr();
//	return 0;
//}
//
//
//#include <chrono>
//#include <thread>
//#include <vector>
//
//
//const unsigned FPS = 1000;
//int c = 0;
//std::vector<char> frameData;
//short cursor = 0;
//
//// Get the intial console buffer.
//auto firstBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
//
//// Create an additional buffer for switching.
//auto secondBuffer = CreateConsoleScreenBuffer(
//		GENERIC_READ | GENERIC_WRITE,
//		FILE_SHARE_WRITE | FILE_SHARE_READ,
//		nullptr,
//		CONSOLE_TEXTMODE_BUFFER,
//		nullptr);
//
//// Assign switchable back buffer.
//HANDLE backBuffer = secondBuffer;
//bool bufferSwitch = true;
//
//// Returns current window size in rows and columns.
//COORD getScreenSize() {
//	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
//	GetConsoleScreenBufferInfo(firstBuffer, &bufferInfo);
//	const auto newScreenWidth = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
//	const auto newscreenHeight = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
//
//	return COORD{ static_cast<short>(newScreenWidth), static_cast<short>(newscreenHeight) };
//}
//
//// Switches back buffer as active.
//void swapBuffers() {
//	WriteConsole(backBuffer, &frameData.front(), static_cast<short>(frameData.size()), nullptr, nullptr);
//	SetConsoleActiveScreenBuffer(backBuffer);
//	backBuffer = bufferSwitch ? firstBuffer : secondBuffer;
//	bufferSwitch = !bufferSwitch;
//	Sleep(1000 / FPS);
////	std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS));
//}
//
//void drawFrame(COORD screenSize) {
//	string s = "Hello \n mom\n dad\n brother\n" + to_string(c++);
//	int j = 1;
//	int offset = 0;
//	for (int i = 0; s[i]; i++) {
//		if (s[i] == '\n') {
//			j++;
//			offset = i + 1;
//			continue;
//		}
//		frameData[j * screenSize.X + i - offset] = s[i];
//	}
//}
//
//int main() {
//	auto screenSize = getScreenSize();
//	SetConsoleScreenBufferSize(firstBuffer, screenSize);
//	SetConsoleScreenBufferSize(secondBuffer, screenSize);
//	frameData.resize(screenSize.X * screenSize.Y);
//	while (true) {
//		auto screenSize2 = getScreenSize();
//		if (screenSize.X != screenSize2.X || screenSize.Y != screenSize2.Y) {
//			SetConsoleScreenBufferSize(secondBuffer, screenSize2);
//			screenSize.X = screenSize2.X;
//			screenSize.Y = screenSize2.Y;
//			frameData.resize(screenSize.X * screenSize.Y);
//			frameData.clear();
//			continue;
//		}
//		drawFrame(screenSize);
//		swapBuffers();
//	}
//}

void project_init();

void enableAnsiSupport() {
	DWORD dwMode = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

const int COL = 5;
const int ROW = 5;

const int LINE = 7;
const int PILAR = 3;

const int ARROW_UP = 0x48;
const int ARROW_LEFT = 0x4B;
const int ARROW_RIGHT = 0x4D;
const int ARROW_DOWN = 0x50;
const int ESC_KEY = 0x1B;
const int ENTER_KEY = 0x0D;

int x = 0;
int y = 0;

struct Node {
	char alphabet;
};

Node nodes[ROW][COL] = {};

struct Selected {
	int x1;
	int x2;
	int y1;
	int y2;
};

Selected chosen{-1, -1, -1, -1};

void drawContentNormal() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);
}

void drawContentCursor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 240);
}

void drawSelect() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 100);
}

char getRandomChar() {
	return (char) (65 + (rand() % (90 - 65 + 1)));
}

void drawLine() {
	cout << string(LINE, '-');
}

void drawContent(char c = ' ') {
	if (c == ' ') cout << string(LINE, ' ');
	else cout << string(LINE / 2, ' ') << c << string((LINE - 1) / 2, ' ');
}

void drawSpaceOut() {
	cout << string(LINE, ' ');
}

void drawPilar() {
	cout << "|";
}

void draw() {
	system("cls");
	cout << "S1: " << chosen.x1 << ":" << chosen.y1 << "\nS2: " << chosen.x2 << ":" << chosen.y2 << "\n";
	for (int i = 0; i < ROW; i++) {
		drawSpaceOut();
		for (int j = 0; j < COL; j++) {
			cout << " ";
			drawLine();
		}
		cout << "\n";
		for (int k = 0; k < PILAR; k++) {
			drawSpaceOut();
			for (int j = 0; j < COL; j++) {
				drawPilar();
				if (y == i && x == j) {
					drawContentCursor();
				}
				if ((chosen.x1 == j && chosen.y1 == i) || (chosen.x2 == j && chosen.y2 == i)) {
					drawSelect();
				}
				if (k == PILAR / 2) {
					drawContent((char) (65 + i * j));
				} else {
					drawContent();
				}
				if ((y == i && x == j) || (chosen.x1 == j && chosen.y1 == i) || (chosen.x2 == j && chosen.y2 == i)) {
					drawContentNormal();
				}
			}
			drawPilar();
			cout << "\n";
		}
	}
	drawSpaceOut();
	for (int j = 0; j < COL; j++) {
		cout << " ";
		drawLine();
	}
}

void project_init() {
	enableAnsiSupport();
	HWND hWnd = GetConsoleWindow();
	ShowScrollBar(hWnd, SB_BOTH, false);
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++)
			Node nodes[i][j];
	}
}

int main() {
	project_init();
	draw();
	unsigned char ch;
	while (true) {
		ch = getch();
		switch (ch) {
			case ARROW_UP:
				if (y > 0) {
					y--;
					draw();
				}
				break;
			case ARROW_LEFT:
				if (x > 0) {
					x--;
					draw();
				}
				break;
			case ARROW_RIGHT:
				if (x + 1 < COL) {
					x++;
					draw();
				}
				break;
			case ARROW_DOWN:
				if (y + 1 < ROW) {
					y++;
					draw();
				}
				break;
			case 'c':
				chosen.x1 = -1;
				chosen.y1 = -1;
				chosen.x2 = -1;
				chosen.y2 = -1;
				draw();
				break;
			case ENTER_KEY:
				if (chosen.x1 == -1 && (chosen.x2 != x || chosen.y2 != y)) {
					chosen.x1 = x;
					chosen.y1 = y;
				} else {
					if (x == chosen.x1 && y == chosen.y1) {
						chosen.x1 = -1;
						chosen.y1 = -1;
					} else {
						if (chosen.x2 == -1) {
							chosen.x2 = x;
							chosen.y2 = y;
						} else {
							if (x == chosen.x2 && y == chosen.y2) {
								chosen.x2 = -1;
								chosen.y2 = -1;
							}
						}
					}
				}
				draw();
				break;
			case ESC_KEY:
				return 0;
//			default:
//				int adjlna = 0;
////				cout << "IDK! " << (int) ch << " : \"" << ch << "\"\n";
		}
	}
	return 0;
}