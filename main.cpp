#pragma clang diagnostic push
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
//	string s = "Hello \nmom\ndad\nbrother\n" + to_string(c++);
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

const string songs[] = {"Mili - String Theocracy.mp3"};
const int SONG_AMOUNT = 1;

void project_init();

void swap(int *&a, int *&b) {
	a = b;
	b = a;
}

int *shuffleSong() {
	int *shuffle = new int[SONG_AMOUNT];
	for (int i = 0; i < SONG_AMOUNT; i++) shuffle[i] = i;
	for (int i = 0; i < SONG_AMOUNT; i++) swap(shuffle[i], shuffle[rand() % SONG_AMOUNT]);
	return shuffle;
}

void deleteShuffle(const int *shuffle) {
	delete[] shuffle;
}
//
//void play(const char *path) {
//	ma_result result;
//	ma_engine engine;
//
//	result = ma_engine_init(NULL, &engine);
//	if (result != MA_SUCCESS) {
//		printf("Failed to initialize audio engine.");
//		exit(1);
//	}
//
//	ma_sound sound;
//	result = ma_sound_init_from_file(&engine, path, 0, NULL, NULL, &sound);
//	if (result != MA_SUCCESS) {
//		printf("Failed to load sound file.");
//		exit(1);
//	}
//	ma_sound_start(&sound);
//	while (ma_sound_at_end(&sound) == 0) {
//		ma_engine_set_volume(&engine, (rand() % 11) / 10.0);
//		string input;
//		getline(cin, input);
//		if (input == "s") break;
//	}
//	ma_sound_stop(&sound);
//	ma_engine_uninit(&engine);
//}

void enableAnsiSupport() {
	DWORD dwMode = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

void project_init() {
	enableAnsiSupport();
}

int main(){
	// Providing a seed value
	srand((unsigned) time(NULL));

	// Get a random number
	for (int i = 0; i < 100; i++) {
		cout << rand() % 11 / 10.0 << "\n";
	}
	return 1;
}

int main1() {
	srand((unsigned) time(nullptr));
	int* shuffle = shuffleSong();
	for (int i = 0; i < SONG_AMOUNT; i++) {
		cout << "Now playing: " << songs[shuffle[i]] << "\n";
//		std::cout.flush();
		play(songs[shuffle[i]].c_str());
	}
	deleteShuffle(shuffle);
	return 0;
}