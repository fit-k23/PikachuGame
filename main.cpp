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

const int COL = 8;
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

int turn = 0;

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

void drawContentSus() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 200);
}

void drawSelect() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 100);
}

char getRandomChar(int range_a = 65, int range_b = 90) {
	return (char) (range_a + (rand() % (range_b - range_a + 1)));
}

void drawLine() {
	cout << string(LINE, '-');
}

void drawContent(char c = ' ') {
	if (c == ' ') cout << string(LINE, ' ');
	else cout << string(LINE / 2, ' ') << c << string((LINE - 1) / 2, ' ');
}

void drawPilar() {
	cout << "|";
}

void draw() {
//	system("cls");
	cout << "\033[2J\033[;H";
	cout << "S1: " << chosen.x1 << ":" << chosen.y1 << "\nS2: " << chosen.x2 << ":" << chosen.y2 << "\n";
	cout << "Turns: " << turn << "\n";
	for (int i = 0; i < MAZE_ROW; i++) {
		for (int j = 0; j < MAZE_COL; j++) {
			cout << ' ';
			if (i != 0 && j != 0 && j != MAZE_COL - 1) drawLine();
			else drawContent();
		}
		cout << ' ';
		cout << "\n";

		for (int k = 0; k < PILAR; k++) {
			for (int j = 0; j < MAZE_COL; j++) {
				if (i != 0 && i != MAZE_ROW - 1 && j != 0) {
					drawPilar();
				} else {
					cout << ' ';
				}
				if (boxes[i - PADDING][j - PADDING].invisible) {
					if (y == i && x == j) {
						drawContentCursor();
					}
					if (!maze[i][j]) {
						drawContentSus();
					}
					drawContent();
					drawContentNormal();
					continue;
				}
				if (y == i && x == j) {
					drawContentCursor();
				}
				if ((chosen.x1 == j && chosen.y1 == i) || (chosen.x2 == j && chosen.y2 == i)) {
					drawSelect();
				}
				if (k == PILAR / 2) {
					if (i != 0 && i != MAZE_ROW - 1 && j != 0 && j != MAZE_COL - 1) drawContent(boxes[i - PADDING][j - PADDING].alphabet);
					else drawContent();
				} else {
					drawContent();
				}
				if ((y == i && x == j) || (chosen.x1 == j && chosen.y1 == i) || (chosen.x2 == j && chosen.y2 == i)) {
					drawContentNormal();
				}
			}
			cout << ' ';
			cout << "\n";
		}
	}
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

const string direction = "DLRU";

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
//
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

struct Coord{
	int x;
	int y;
};

struct QueueNode{
	Coord coord;
	int r;
	int old_dr;
	int turns;
};

// BFS Algorithm
int findPath(Coord src, Coord dest) {
	// check source and destination cell of the matrix have value 1
	if (!maze[src.y][src.x] || !maze[dest.y][dest.x]) {
		system("start cmd");
		return -1;
	}

	bool visited[MAZE_ROW][MAZE_COL];
	memset(visited, false, sizeof visited);

	// Mark the source cell as visited
	visited[src.y][src.x] = true;

	// Create a queue for BFS
	queue<QueueNode> q;

	// Distance of source cell is 0
	QueueNode s = {src, 0, -1,0};
	q.push(s);  // Enqueue source cell

	// Do a BFS starting from source cell
	while (!q.empty()) {
		QueueNode curr = q.front();
		Coord coord = curr.coord;

		// If we have reached the destination cell,
		// we are done
		if (curr.turns <= 2 && coord.x == dest.x && coord.y == dest.y) {
			turn = curr.turns;
			return curr.r;
		}

		// Otherwise dequeue the front
		// cell in the queue
		// and enqueue its adjacent cells
		q.pop();

		for (int i = 0; i < 4; i++) {
			if (i == 3 - curr.old_dr) continue;
			int row = coord.y + dr[i];
			int col = coord.x + dc[i];

			// if adjacent cell is valid, has path and
			// not visited yet, enqueue it.
			if (isValid(row, col) && maze[row][col] &&
				!visited[row][col]) {
				// mark cell as visited and enqueue it
				int newTurns = curr.turns;
				if (curr.old_dr != -1 && i != curr.old_dr) newTurns++;
				visited[row][col] = true;
//				if (newTurns > 2) {
//					string s = "Turn: " + to_string(newTurns);
//					system(("start cmd /k echo " + s).c_str());
//					visited[row][col] = false;
//					continue;
//				}
				QueueNode Adjcell = {
					{col, row},
					curr.r + 1,
					i,
					newTurns
				};
				q.push(Adjcell);
			}
		}
	}
	// Return -1 if destination cannot be reached
	return -1;
}

///////////////////////////////////////////////////////////

void project_init() {
	ShowWindow(GetConsoleWindow(),SW_MAXIMIZE);
	enableAnsiSupport();
	HWND hWnd = GetConsoleWindow();
	ShowScrollBar(hWnd, SB_BOTH, false);

	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);

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

int main() {
	project_init();
	draw();

	unsigned char ch;
	while (true) {
		ch = getch();
		switch (ch) {
			case ARROW_UP:
			case 'w':
				if (y > 1) {
					y--;
					draw();
				}
				break;
			case ARROW_LEFT:
			case 'a':
				if (x > 1) {
					x--;
					draw();
				}
				break;
			case ARROW_RIGHT:
			case 'd':
				if (x + 1 - PADDING < COL) {
					x++;
					draw();
				}
				break;
			case ARROW_DOWN:
			case 's':
				if (y + 1 - PADDING < ROW) {
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
				if (maze[y][x]) break;
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
				if (chosen.x1 != -1 && chosen.x2 != -1) {
					if (boxes[chosen.y1 - PADDING][chosen.x1 - PADDING].alphabet == boxes[chosen.y2 - PADDING][chosen.x2 - PADDING].alphabet) {
						bool temp = (chosen.x1 == chosen.x2 && abs(chosen.y1 - chosen.y2) == 1) || (chosen.y1 == chosen.y2 && abs(chosen.x1 - chosen.x2) == 1);
						maze[chosen.y1][chosen.x1] = true;
						maze[chosen.y2][chosen.x2] = true;
						if (temp || findPath(Coord{chosen.x1, chosen.y1}, Coord{chosen.x2, chosen.y2}) != -1) {// || findPath(Coord{chosen.x2, chosen.y2}, Coord{chosen.x1, chosen.y1}) != -1) {
							boxes[chosen.y1 - PADDING][chosen.x1 - PADDING].invisible = true;
							boxes[chosen.y2 - PADDING][chosen.x2 - PADDING].invisible = true;
						} else {
							maze[chosen.y1][chosen.x1] = false;
							maze[chosen.y2][chosen.x2] = false;
						}
					}
					chosen.x1 = -1;
					chosen.y1 = -1;
					chosen.x2 = -1;
					chosen.y2 = -1;
				}
				draw();
				printMaze();
				break;
			case ESC_KEY:
				return 0;
		}
	}
}