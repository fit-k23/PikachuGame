#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cert-msc50-cpp"

#include <queue>
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
////char* frameData = new char[SIZE];
//std::vector<char> frameData;
//short cursor = 0;
//
//// Get the initial console buffer.
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

const int COL = 12;
const int ROW = 8;

const int LINE = 7;
const int PILAR = 3;

const int ARROW_UP = 0x48;
const int ARROW_LEFT = 0x4B;
const int ARROW_RIGHT = 0x4D;
const int ARROW_DOWN = 0x50;
const int ESC_KEY = 0x1B;
const int ENTER_KEY = 0x0D;

struct Box {
	char alphabet = ' ';
	bool invisible = false;
};

const int PADDING = 1;

const int MAZE_ROW = ROW + 2 * PADDING;
const int MAZE_COL = COL + 2 * PADDING;

Box boxes[ROW][COL] = {};
bool maze[MAZE_ROW][MAZE_COL];

int x = PADDING;
int y = PADDING;

struct Selected {
	int x1;
	int x2;
	int y1;
	int y2;
};

Selected chosen{-1, -1, -1, -1};

char getRandomChar(int range_a = 65, int range_b = 90) {
	return (char) (range_a + (rand() % (range_b - range_a + 1)));
}

const char* CLEAR_ANSI = "\033[2J\033[;H";
//const char* NORMAL_ANSI = "\033[39\033[49";
const char* NORMAL_ANSI = "\033[48;2;0;0;0m\033[38;2;255;255;255m";
const char* SELECT_COLOR_ANSI = "\033[48;2;123;23;131m";
const char* CURSOR_COLOR_ANSI = "\033[48;2;255;255;255m\x1B[38;2;0;0;0m";


void drawLine(ostringstream &sstr) {
	sstr << string(LINE, '-');
}

void drawNormalBoxContent(ostringstream &sstr, char c = ' ') {
	if (c == ' ') sstr << string(LINE, ' ');
	else sstr << string(LINE / 2, ' ') << c << string((LINE - 1) / 2, ' ');
}

void drawPilar(ostringstream &sstr) {
	sstr << "|";
}

void MoveCursorTo(COORD coord) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdout, coord);
}

void draw() {
//	system("cls");
	ostringstream sstr;
	char a[1000 * 1000] = "";
	MoveCursorTo({0,0});
//	sstr << CLEAR_ANSI;
	sstr << "S1: " << chosen.x1 << ":" << chosen.y1 << "\nS2: " << chosen.x2 << ":" << chosen.y2 << "\n";
	for (int i = 0; i < MAZE_ROW; i++) {
		for (int j = 0; j < MAZE_COL; j++) {
			sstr << ' ';
			if (i != 0 && j != 0 && j != MAZE_COL - 1)  drawLine(sstr);//&& !boxes[i - PADDING][j - PADDING].invisible && !boxes[i - PADDING + 1][j - PADDING].invisible) drawLine(sstr);
			else drawNormalBoxContent(sstr);
		}
		sstr << " \n";

		for (int k = 0; k < PILAR; k++) {
			for (int j = 0; j < MAZE_COL; j++) {
				if (i != 0 && i != MAZE_ROW - 1 && j != 0) {// && !boxes[i - PADDING][j - PADDING].invisible) {
//					strcat(a, "|");
					drawPilar(sstr);
				} else {
//					strcat(a, " ");
					sstr << ' ';
				}
				if (boxes[i - PADDING][j - PADDING].invisible) {
					if (y == i && x == j) {
//						strcat(a, CURSOR_COLOR_ANSI);
						sstr << CURSOR_COLOR_ANSI;
					}
					drawNormalBoxContent(sstr);
					sstr << NORMAL_ANSI;
					continue;
				}
				if (y == i && x == j) {
					sstr << CURSOR_COLOR_ANSI;
				}
				if ((chosen.x1 == j && chosen.y1 == i) || (chosen.x2 == j && chosen.y2 == i)) {
					sstr << SELECT_COLOR_ANSI;
				}
				if (k == PILAR / 2) {
					if (i != 0 && i != MAZE_ROW - 1 && j != 0 && j != MAZE_COL - 1) drawNormalBoxContent(sstr, boxes[i - PADDING][j - PADDING].alphabet);
					else drawNormalBoxContent(sstr);
				} else {
					drawNormalBoxContent(sstr);
				}
				if ((y == i && x == j) || (chosen.x1 == j && chosen.y1 == i) || (chosen.x2 == j && chosen.y2 == i)) {
					sstr << NORMAL_ANSI;
				}
			}
			sstr << " \n";
		}
	}
	cout << sstr.str();
}

///////////////////////////////////////////////////////////
// Maze zone

void resetMaze() {
	for (int i = 0; i < MAZE_ROW; i++) {
		for (int j = 0; j < MAZE_COL; j++) {
			if (i == 0 || i == MAZE_ROW - 1 || j == 0 || j == MAZE_COL -1) {
				maze[i][j] = true;
			} else {
				maze[i][j] = boxes[i - PADDING][j - PADDING].invisible;
			}
		}
	}
}

const char DIRECTION[] = "DLRU";

// Arrays to represent change in rows and columns
// DOWN, LEFT, RIGHT, UP
// 0 1 2 3
// 0 - 3 DOWN - UP -> 3 - 0 = 3
// 1 - 2 LEFT - RIGHT -> 3 - 2 = 1

const int dr[4] = {1, 0, 0, -1};
const int dc[4] = {0, -1, 1, 0};

bool isValid(int row, int col) {
	return row >= 0 && col >= 0 && row < MAZE_ROW && col < MAZE_COL;
}

struct Coord{
	int x;
	int y;
};

struct QueueNode{
	Coord coord;
	int old_dr;
	int turns;
};


void print2DArray(int a[MAZE_ROW][MAZE_COL], Coord src = {-1, -1}, Coord dist = {-1,-1}) {
	for (int i = 0; i < MAZE_ROW; i++) {
		for (int j = 0; j < MAZE_COL; j++) {
			if (i == dist.y && j == dist.x) cout << "\x1B[48;2;123;23;131m";
			if (i == src.y && j == src.x) cout << "\x1B[48;2;223;23;23m";
			cout << a[i][j] << NORMAL_ANSI << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int getBannedDirection(Coord &src, Coord &dest) {
	int predicted_y_direction = (src.y - dest.y) / abs(src.y - dest.y);
	int predicted_x_direction = (src.x - dest.x) / abs(src.x - dest.x);
	cout << predicted_x_direction << " " << predicted_y_direction << "\n";
	if (!maze[src.y][src.x - predicted_x_direction]) {
		if (!maze[src.y - predicted_y_direction][src.x]) {
			return -1;
		}
		return predicted_x_direction > 0 ? 2 : 1;
	}
	if (!maze[src.y - predicted_y_direction][src.x]) {
		if (!maze[src.y][src.x - predicted_x_direction]) {
			return -1;
		}
		return predicted_y_direction > 0 ? 0 : 3;
	}
	return -1;
}

int getBannedYDirection(Coord &src, Coord &dest) {
	if (src.y == dest.y) return -1;
	if (src.x == dest.x) return -1;
	int predicted_y_direction = (src.y - dest.y) / abs(src.y - dest.y);
	return maze[src.y + predicted_y_direction][src.x] ? -1 : predicted_y_direction > 0 ? 0 : 3;
}

int getBannedXDirection(Coord &src, Coord &dest) {
	if (src.y == dest.y) return -1;
	if (src.x == dest.x) return -1;
	int predicted_x_direction = (src.x - dest.x) / abs(src.x - dest.x);
	return maze[src.y][src.x + predicted_x_direction] ? -1 : predicted_x_direction > 0 ? 2 : 1;
}

// Modified BFS Algorithm
int findPath(Coord src, Coord dest, string &path) {
	int c = 0;

	bool checkDest = false;
	for (int i = 0; i < 4; i++) {
		int row = dest.y + dr[i];
		int col = dest.x + dc[i];
		if (isValid(row, col) && maze[row][col]) {
			checkDest = true;
		}
	}
	if (!checkDest) return -1;

	if (!maze[src.y][src.x] || !maze[dest.y][dest.x]) {
		return -1;
	}

	int turnMap[MAZE_ROW][MAZE_COL];
	memset(turnMap, 0, sizeof turnMap);
	turnMap[src.y][src.x] = 0;

	int bd = getBannedDirection(src, dest);
	int bd_x = getBannedXDirection(src, dest);
	int bd_y = getBannedYDirection(src, dest);
	cout << bd_x << "\n" << bd_y << "\n---\n";
	queue<QueueNode> q;
	q.push({src, -1, 0});
	path = " ";

	while (!q.empty()) {
		auto cur = q.front();
		Coord coord = cur.coord;
		if (coord.x == dest.x && coord.y == dest.y) {
			print2DArray(turnMap, src, dest);
			return 1;
		}
		q.pop();
		path += " > ";
//		path.pop_back();
		for (int i = 0; i < 4; i++) {
			if (i == 3 - cur.old_dr) {
//			if (i == 3 - cur.old_dr || i == bd_x) {
//			if (i == 3 - cur.old_dr || i == bd_x || i == bd_y) {
				//cout << "HERE0!\n";
				continue;
			}
			int row = coord.y + dr[i];
			int col = coord.x + dc[i];

			if (isValid(row, col) && maze[row][col]) {
				int tPoint = cur.turns;
				if (cur.old_dr != -1 && i != cur.old_dr) tPoint++;
				if (tPoint > 2) {
//					if (col == dest.x && row == dest.y) {
//						print2DArray(turnMap, src, dest);
//						//cout << "HERE1!\n";
//						continue;
//					}
					//cout << "HERE3!\n";
					continue;
				}
				if (tPoint == 2 && (row != dest.y && col != dest.x)) {
					//cout << "HERE4!\n";
					continue;
				}
				turnMap[row][col] = tPoint;
//				print2DArray(turnMap, src, dest);
				path += DIRECTION[i];
				q.push({{col, row}, i, tPoint});
				c++;
			}
		}
	}
	//cout << "HERE2!\n";
	print2DArray(turnMap, src, dest);
	return -1;
}

//void findPath(int x1, int y1, int x2, int y2, vector<string> &ans, string &currentPath, int skip = -1) {
//	if (x1 == x2 && y1 == y2) {
//		ans.push_back(currentPath);
//		return;
//	}
//	maze[y1][x1] = false;
//
//	for (int i = 0; i < 4; i++) {
//		if (i == skip) continue;
//		int nextCol = x1 + dc[i];
//		int nextRow = y1 + dr[i];
//
//		if (!maze[nextRow][nextCol] && !(nextCol = x2 && nextRow == y2)) {
//			continue;
//		}
//
//		if (isValid(nextCol, nextRow)) {
//			currentPath += direction[i];
//			findPath(nextCol, nextRow, x2, y2, ans,currentPath, 3 - i);
//			currentPath.pop_back();
//		}
//		maze[y1][x1] = false;
//		maze[y2][x2] = false;
//	}
//}

bool findPathAlter(Coord src, Coord dest) {
	if (!maze[src.y][src.x] || !maze[dest.y][dest.x]) {
		return false;
	}
	return false;
}

///////////////////////////////////////////////////////////

void DisableResizeWindow() {
	HWND hWnd = GetConsoleWindow();
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void DisableMinimizeButton() {
	HWND hWnd = GetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hWnd, false);

	DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
}

void project_init() {
	SetConsoleOutputCP(65001);
//	system("mode con COLS=700");
	SetConsoleTitleW(L"Pikachu Game");
	ShowWindow(GetConsoleWindow(),SW_MAXIMIZE);
//	SendMessage(GetConsoleWindow(),WM_SYSKEYDOWN,VK_RETURN,0x20000000); Full screen
	enableAnsiSupport();
	HWND hWnd = GetConsoleWindow();
//	ShowScrollBar(hWnd, SB_BOTH, false);

	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);

//	DisableResizeWindow(); //auto resize
	DisableMinimizeButton();
//	DeleteMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);

	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
	SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

//	DeleteMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_MINIMIZE, MF_BYCOMMAND);

//	memset(maze, false, (ROW + 2*PADDING) * (COL + 2*PADDING));
	for (auto & box : boxes) {
		for (auto & j : box) {
			j.alphabet = getRandomChar(65, 70);
		}
	}
	resetMaze();
}

void printMaze() {
	for (auto & i : maze) {
		for (bool j : i) {
			cout << (j ? "1 " : "0 ");
		}
		cout << "\n";
	}
	cout << "\n---------------\n";
}

void project_cleanup() {
//	delete[] CLEAR_ANSI;
//	delete[] NORMAL_ANSI;
//	delete[] CURSOR_COLOR_ANSI;
//	delete[] SELECT_COLOR_ANSI;
	system("pause");
}

bool FLAG_RUNNING = true;

int main() {
	project_init();

	const char* s1 = "Meow";
	const char* s2 = string(s1).append(" Goofiest").c_str();
	//cout << s2 << "\n";

	draw();
	unsigned char ch;
	while (FLAG_RUNNING) {
		ch = getch();
		switch (ch) {
			case ARROW_UP:
			case 'w':
				if (y > 0) {
					y--;
					if (y == 0) y = ROW + PADDING - 1;
					draw();
				}
				break;
			case ARROW_LEFT:
			case 'a':
				if (x > 0) {
					x--;
					if (x == 0) x = COL + PADDING - 1;
					draw();
				}
				break;
			case ARROW_RIGHT:
			case 'd':
				if (x < COL + PADDING) {
					x++;
					if (x == COL + PADDING) x = PADDING;
					draw();
				}
				break;
			case ARROW_DOWN:
			case 's':
				if (y < ROW + PADDING) {
					y++;
					if (y == ROW + PADDING) y = PADDING;
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
				if (maze[y][x]) break;
				if (chosen.x1 == -1 && (chosen.x2 != x || chosen.y2 != y)) {
					chosen.x1 = x;
					chosen.y1 = y;
				} else if (x == chosen.x1 && y == chosen.y1) {
					chosen.x1 = -1;
					chosen.y1 = -1;
				} else if (chosen.x2 == -1) {
					chosen.x2 = x;
					chosen.y2 = y;
				} else if (x == chosen.x2 && y == chosen.y2) {
					chosen.x2 = -1;
					chosen.y2 = -1;
				}
				draw();
				if (chosen.x1 != -1 && chosen.x2 != -1) {
					if (boxes[chosen.y1 - PADDING][chosen.x1 - PADDING].alphabet == boxes[chosen.y2 - PADDING][chosen.x2 - PADDING].alphabet) {
						bool temp = (chosen.x1 == chosen.x2 && abs(chosen.y1 - chosen.y2) == 1) || (chosen.y1 == chosen.y2 && abs(chosen.x1 - chosen.x2) == 1);
						maze[chosen.y1][chosen.x1] = true;
						maze[chosen.y2][chosen.x2] = true;
						string tempString;
						if (temp || findPath(Coord{chosen.x1, chosen.y1}, Coord{chosen.x2, chosen.y2}, tempString) != -1) {
							//cout << tempString << "\n--------------\n";
							Sleep(50);
							boxes[chosen.y1 - PADDING][chosen.x1 - PADDING].invisible = true;
							boxes[chosen.y2 - PADDING][chosen.x2 - PADDING].invisible = true;
						} else {
							Sleep(1);
							maze[chosen.y1][chosen.x1] = false;
							maze[chosen.y2][chosen.x2] = false;
						}
					}
					chosen.x1 = -1;
					chosen.y1 = -1;
					chosen.x2 = -1;
					chosen.y2 = -1;
					draw();
				}
				break;
			case ESC_KEY:
				FLAG_RUNNING = false;
		}
	}
	project_cleanup();
	return 0;
}