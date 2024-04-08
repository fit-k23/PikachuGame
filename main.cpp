#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cert-msc50-cpp"

#include "main.h"

using namespace std;

int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;

COORD getScreenSize() {
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);
	const auto newScreenWidth = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
	const auto newscreenHeight = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;

	return COORD{static_cast<short>(newScreenWidth), static_cast<short>(newscreenHeight)};
}

void syncScrSize() {
	COORD scrSize = getScreenSize();
	SCREEN_WIDTH = scrSize.X;
	SCREEN_HEIGHT = scrSize.Y;
}

void enableAnsiSupport() {
	DWORD dwMode = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

const int COL = 10;
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

Box** boxes;
bool** maze;

struct Coord{
	int x = -1;
	int y = -1;
	bool isEqual(Coord c) const;
};

bool Coord::isEqual(Coord c) const {
	return c.x == x && c.y == y;
}

Coord cursor = {PADDING, PADDING};

struct Selector {
	Coord c1;
	Coord c2;
	bool valid = false;
};

Selector selector{{-1, -1}, {-1, -1}, false};

const char* NORMAL_ANSI = "\033[38;2;255;255;255m\033[49m";
const char* SELECT_COLOR_ANSI = "\033[48;2;123;23;131m";
const char* CURSOR_COLOR_ANSI = "\033[48;2;255;255;255m\x1B[38;2;0;0;0m";

void drawLine(ostringstream &sstr) {
//	sstr << string(LINE, '-');
	sstr << str_repeat("─", LINE);
}

void drawNormalBoxContent(ostringstream &sstr, char c = ' ') {
	if (c == ' ') sstr << string(LINE, ' ');
	else sstr << string(LINE / 2, ' ') << c << string((LINE - 1) / 2, ' ');
}

void drawPilar(ostringstream &sstr) {
//	sstr << "|";
	sstr << "│";
}

void MoveCursorTo(COORD coord) {
	if (coord.X > SCREEN_WIDTH || coord.Y > SCREEN_HEIGHT) return;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdout, coord);
}

void drawDoubleLayerRectangle(COORD coord, int width, int height) {
	MoveCursorTo(coord);
	string s = "╔";
	string space;
	string temp;
	for (int i = 0; i < width; i++) {
		s += "═";
	}
	cout << s << "╗";

	for (int i = 0; i < height / 2; i++) {
		MoveCursorTo({coord.X, static_cast<SHORT>(coord.Y + i + 1)});

//		cout << "\033[48;2;" << rand() % 255 <<  rand() % 255
//		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
		printf(("║\033[48;2;%d;%d;%dm" + string(width, ' ') + "\033[39m\033[49m║").c_str(), rand() % 256, rand() % 256, rand() % 256);
//		cout << "║" + string(width, ' ') + "║";
//		printf("\033[39m\033[49m");
	}
	s = "╚";
	for (int i = 0; i < width; i++)
		s += "═";
	MoveCursorTo({coord.X, static_cast<SHORT>(coord.Y + height / 2 + 1)});
	s += "╝";
	cout << s;
	MoveCursorTo({0, 50});
}

void drawThinRectangle(COORD coord, int width, int height) {
	MoveCursorTo(coord);
	string s = "┌";
	string space;
	string temp;
	for (int i = 0; i < width; i++) {
		s += "─";
	}
	cout << s << "┐";

	for (int i = 0; i < height / 2; i++) {
		MoveCursorTo({coord.X, static_cast<SHORT>(coord.Y + i + 1)});
		cout << "│" + string(width, ' ') + "│";
	}
	s = "└";
	for (int i = 0; i < width; i++)
		s += "─";
	MoveCursorTo({coord.X, static_cast<SHORT>(coord.Y + height / 2 + 1)});
	s += "┘";
	cout << s;
	MoveCursorTo({0, 50});
}

void drawAtPos(COORD coord, const string& s) {
	istringstream sstr(s);
	string temp;
	while (!sstr.eof()) {
		MoveCursorTo(coord);
		getline(sstr, temp, '\n');
		printf("%s", temp.c_str());
		coord.Y++;
	}
}



void draw() {
	//drawAtPos({18,9}, getFileContent("poké-side-of-the-block.txt"));
	ostringstream sstr;
	MoveCursorTo({0,0});
//	sstr << "S1: " << selector.c1.x << ":" << selector.c1.y << "\nS2: " << selector.c2.x << ":" << selector.c2.y << "\n";
	for (int i = 0; i < MAZE_ROW; i++) {
		for (int j = 0; j < MAZE_COL; j++) {
			sstr << ' ';

			if (i != 0 && i != MAZE_ROW - 1) {
//				if (i == MAZE_ROW - 2 && boxes[i - PADDING - 1][j - PADDING].invisible) drawNormalBoxContent(sstr);
				if (i == PADDING && boxes[i - PADDING][j - PADDING].invisible) drawNormalBoxContent(sstr);
				else if (i >= 2 && boxes[i - PADDING][j - PADDING].invisible && boxes[i - PADDING - 1][j - PADDING].invisible) drawNormalBoxContent(sstr);
				else if (j != 0 && j != MAZE_COL - 1) drawLine(sstr);
				else drawNormalBoxContent(sstr);
			} else {
				if (j != 0 && j != MAZE_COL - 1 && i == MAZE_ROW - 1 && !boxes[i - PADDING - 1][j - PADDING].invisible) drawLine(sstr);
				else drawNormalBoxContent(sstr);
			}
		}
		sstr << " \n";

		for (int k = 0; k < PILAR; k++) {
			for (int j = 0; j < MAZE_COL; j++) {
				if (i != 0 && i != MAZE_ROW - 1 && j != 0) {
					if (j == MAZE_COL - 1 && boxes[i - PADDING][j - PADDING - 1].invisible) sstr << ' ';
					else if (j == 1 && boxes[i - PADDING][j - PADDING].invisible) sstr << ' ';
					else if (j >= 2 && boxes[i - PADDING][j - PADDING].invisible && boxes[i - PADDING][j - PADDING - 1].invisible) sstr << ' ';
					else drawPilar(sstr);
				} else {
					sstr << ' ';
				}
				if (i >= 1 && j >= 1 && i <= MAZE_ROW - 2 && j <= MAZE_COL - 2 && boxes[i - PADDING][j - PADDING].invisible) {
					if (cursor.isEqual({j, i})) {
						sstr << CURSOR_COLOR_ANSI;
					}
					drawNormalBoxContent(sstr);
					sstr << NORMAL_ANSI;
					continue;
				}
//				if (y == i && x == j) {
				if (cursor.isEqual({j, i})) {
					sstr << CURSOR_COLOR_ANSI;
				}
				if ((selector.c1.x == j && selector.c1.y == i) || (selector.c2.x == j && selector.c2.y == i)) {
					sstr << SELECT_COLOR_ANSI;
				}
				if (k == PILAR / 2) {
					if (i != 0 && i != MAZE_ROW - 1 && j != 0 && j != MAZE_COL - 1) drawNormalBoxContent(sstr, boxes[i - PADDING][j - PADDING].alphabet);
					else drawNormalBoxContent(sstr);
				} else {
					drawNormalBoxContent(sstr);
				}
				if (cursor.isEqual({j, i}) || selector.c1.isEqual({j, i}) || selector.c2.isEqual({j, i})) {
					sstr << NORMAL_ANSI;
				}
			}
			sstr << " \n";
		}
	}
	cout << sstr.str();
	SHORT i = 30;
	MoveCursorTo({160, i++});
	cout << "┌───────────────────────────────────┐";
	MoveCursorTo({160, i++});
	cout << "│ > Use arrow keys for navigating!  │";
	MoveCursorTo({160, i++});
	cout << "│ > Toggle ENTER to select/deselect!│";
	MoveCursorTo({160, i++});
	cout << "└───────────────────────────────────┘";
	MoveCursorTo({160, i++});
	cout << ("╒════════╕");
	MoveCursorTo({160, i++});
	cout << ("│ Hello  │");
	MoveCursorTo({160, i++});
	cout << ("│ world  │");
	MoveCursorTo({160, i++});
	cout << ("╘════════╛");
	//drawDoubleLayerRectangle({160, 10}, 80, 20);
	//drawThinRectangle({160, 50}, 20, 5);
}


void drawOld() {
	//drawAtPos({18,9}, getFileContent("poké-side-of-the-block.txt"));
	ostringstream sstr;
	MoveCursorTo({0,0});
//	sstr << "S1: " << selector.c1.x << ":" << selector.c1.y << "\nS2: " << selector.c2.x << ":" << selector.c2.y << "\n";
	for (int i = 0; i < MAZE_ROW; i++) {
		for (int j = 0; j < MAZE_COL; j++) {
			sstr << ' ';

			if (i != 0 && i != MAZE_ROW - 1) {
//				if (i == MAZE_ROW - 2 && boxes[i - PADDING - 1][j - PADDING].invisible) drawNormalBoxContent(sstr);
				if (i == PADDING && boxes[i - PADDING][j - PADDING].invisible) drawNormalBoxContent(sstr);
				else if (i >= 2 && boxes[i - PADDING][j - PADDING].invisible && boxes[i - PADDING - 1][j - PADDING].invisible) drawNormalBoxContent(sstr);
				else if (j != 0 && j != MAZE_COL - 1) drawLine(sstr);
				else drawNormalBoxContent(sstr);
			} else {
				if (j != 0 && j != MAZE_COL - 1 && i == MAZE_ROW - 1 && !boxes[i - PADDING - 1][j - PADDING].invisible) drawLine(sstr);
				else drawNormalBoxContent(sstr);
			}
		}
		sstr << " \n";

		for (int k = 0; k < PILAR; k++) {
			for (int j = 0; j < MAZE_COL; j++) {
				if (i != 0 && i != MAZE_ROW - 1 && j != 0) {
					if (j == MAZE_COL - 1 && boxes[i - PADDING][j - PADDING - 1].invisible) sstr << ' ';
					else if (j == 1 && boxes[i - PADDING][j - PADDING].invisible) sstr << ' ';
					else if (j >= 2 && boxes[i - PADDING][j - PADDING].invisible && boxes[i - PADDING][j - PADDING - 1].invisible) sstr << ' ';
					else drawPilar(sstr);
				} else {
					sstr << ' ';
				}
				if (i >= 1 && j >= 1 && i <= MAZE_ROW - 2 && j <= MAZE_COL - 2 && boxes[i - PADDING][j - PADDING].invisible) {
					if (cursor.isEqual({j, i})) {
						sstr << CURSOR_COLOR_ANSI;
					}
					drawNormalBoxContent(sstr);
					sstr << NORMAL_ANSI;
					continue;
				}
//				if (y == i && x == j) {
				if (cursor.isEqual({j, i})) {
					sstr << CURSOR_COLOR_ANSI;
				}
				if ((selector.c1.x == j && selector.c1.y == i) || (selector.c2.x == j && selector.c2.y == i)) {
					sstr << SELECT_COLOR_ANSI;
				}
				if (k == PILAR / 2) {
					if (i != 0 && i != MAZE_ROW - 1 && j != 0 && j != MAZE_COL - 1) drawNormalBoxContent(sstr, boxes[i - PADDING][j - PADDING].alphabet);
					else drawNormalBoxContent(sstr);
				} else {
					drawNormalBoxContent(sstr);
				}
				if (cursor.isEqual({j, i}) || selector.c1.isEqual({j, i}) || selector.c2.isEqual({j, i})) {
					sstr << NORMAL_ANSI;
				}
			}
			sstr << " \n";
		}
	}
	cout << sstr.str();
	SHORT i = 30;
	MoveCursorTo({160, i++});
	cout << "┌───────────────────────────────────┐";
	MoveCursorTo({160, i++});
	cout << "│ > Use arrow keys for navigating!  │";
	MoveCursorTo({160, i++});
	cout << "│ > Toggle ENTER to select/deselect!│";
	MoveCursorTo({160, i++});
	cout << "└───────────────────────────────────┘";
	MoveCursorTo({160, i++});
	cout << ("╒════════╕");
	MoveCursorTo({160, i++});
	cout << ("│ Hello  │");
	MoveCursorTo({160, i++});
	cout << ("│ world  │");
	MoveCursorTo({160, i++});
	cout << ("╘════════╛");
	drawDoubleLayerRectangle({160, 10}, 80, 20);
	drawThinRectangle({160, 50}, 20, 5);
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

// Arrays to represent change in rows and columns
// DOWN, LEFT, RIGHT, UP
// 0 1 2 3
// 0 - 3 DOWN - UP -> Counter: 3 - 0 = 3
// 1 - 2 LEFT - RIGHT -> Counter: 3 - 2 = 1

const int dr[4] = {1, 0, 0, -1};
const int dc[4] = {0, -1, 1, 0};

struct Path{
	Coord corners[4];
	int direction;
	int turns = 0;
};

bool isValid(int row, int col) {
	return row >= 0 && col >= 0 && row < MAZE_ROW && col < MAZE_COL;
}

// Modified Breadth-First-Search (BFS) Algorithm
// A common graph traversal algorithm
// Based on the docs provided by GeeksForGeeks: https://www.geeksforgeeks.org/breadth-first-search-or-bfs-for-a-graph/

Path findPath(Coord src, Coord dest) {
	if ((src.x == dest.x && abs(src.y - dest.y) == 1) || (src.y == dest.y && abs(src.x - dest.x) == 1)) {
		return Path{{dest}, -1, 0};
	}
	if (!maze[src.y][src.x] || !maze[dest.y][dest.x]) {
		return Path{{}, -1, -1};
	}
	queue<Path> q;
	q.push({{src}, -1, 0});
	while (!q.empty()) {
		auto cur = q.front();
		Coord coord = cur.corners[cur.turns];
		q.pop();
		for (int move = 0; move < 4; move++) {
			if (move == 3 - cur.direction) {
				continue;
			}
			int row = coord.y + dr[move];
			int col = coord.x + dc[move];
			if (src.isEqual({col, row})) continue;

			if (isValid(row, col) && maze[row][col]) {
//					MoveCursorTo({static_cast<SHORT>((1 + LINE) * coord.x + 1), static_cast<SHORT>((1 + PILAR) * coord.y + 1)});
//					cout << "\033[48;2;100;245;245m" << string(LINE, ' ') << "\033[049m";
//					MoveCursorTo({static_cast<SHORT>((1 + LINE) * coord.x + 1), static_cast<SHORT>((1 + PILAR) * coord.y + 2)});
//					cout << "\033[48;2;100;245;245m" << string(LINE, ' ') << "\033[049m";
//					MoveCursorTo({static_cast<SHORT>((1 + LINE) * coord.x + 1), static_cast<SHORT>((1 + PILAR) * coord.y + 3)});
//					cout << "\033[48;2;100;245;245m" << string(LINE, ' ') << "\033[049m";
//
//					MoveCursorTo({static_cast<SHORT>((1 + LINE) * col + 1), static_cast<SHORT>((1 + PILAR) * row + 1)});
//					cout << "\033[48;2;0;245;25m" << string(LINE, ' ') << "\033[049m";
//					MoveCursorTo({static_cast<SHORT>((1 + LINE) * col + 1), static_cast<SHORT>((1 + PILAR) * row + 2)});
//					cout << "\033[48;2;0;245;25m" << string(LINE, ' ') << "\033[049m";
//					MoveCursorTo({static_cast<SHORT>((1 + LINE) * col + 1), static_cast<SHORT>((1 + PILAR) * row + 3)});
//					cout << "\033[48;2;0;245;25m" << string(LINE, ' ') << "\033[049m";

//					Sleep(100);
//
//				MoveCursorTo({static_cast<SHORT>((1 + LINE) * coord.x + 1), static_cast<SHORT>((1 + PILAR) * coord.y + 1)});
//				cout << "\033[049m" << string(LINE, ' ') << "\033[049m";
//				MoveCursorTo({static_cast<SHORT>((1 + LINE) * coord.x + 1), static_cast<SHORT>((1 + PILAR) * coord.y + 2)});
//				cout << "\033[049m" << string(LINE, ' ') << "\033[049m";
//				MoveCursorTo({static_cast<SHORT>((1 + LINE) * coord.x + 1), static_cast<SHORT>((1 + PILAR) * coord.y + 3)});
//				cout << "\033[049m" << string(LINE, ' ') << "\033[049m";
//
//					MoveCursorTo({static_cast<SHORT>((1 + LINE) * col + 1), static_cast<SHORT>((1 + PILAR) * row + 1)});
//					cout << "\033[049m" << string(LINE, ' ') << "\033[049m";
//					MoveCursorTo({static_cast<SHORT>((1 + LINE) * col + 1), static_cast<SHORT>((1 + PILAR) * row + 2)});
//					cout << "\033[049m" << string(LINE, ' ') << "\033[049m";
//					MoveCursorTo({static_cast<SHORT>((1 + LINE) * col + 1), static_cast<SHORT>((1 + PILAR) * row + 3)});
//					cout << "\033[049m" << string(LINE, ' ') << "\033[049m";

				int turn = cur.turns;
				if (cur.direction != -1 && move != cur.direction) {
					turn++;
				}
				if (turn > 2) {
					continue;
				}
				if (turn == 2 && col != dest.x && row != dest.y) {
					continue;
				}
				MoveCursorTo({80, 40});
				cout << "Turns: " << turn << "    " + to_string(rand()) + "     ";
				Path newPath{{}, move, turn};

				for (int i = 0; i <= cur.turns; i++) {
					newPath.corners[i] = cur.corners[i];
				}

				newPath.corners[turn] = {col, row};

				if (col == dest.x && row == dest.y) {
					return newPath;
				}
				q.push(newPath);
				//Sleep(1000);
			}
		}
	}
	return Path{{}, -1, -1};
}


void disableResizeWindow() {
	HWND hWnd = GetConsoleWindow();
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void disableMinimizeButton() {
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


	syncScrSize();

//	DeleteMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_MINIMIZE, MF_BYCOMMAND);

	boxes = new Box*[ROW];
	for (int i = 0; i < ROW; i++) boxes[i] = new Box[COL];

	maze = new bool*[MAZE_ROW];
	for (int i = 0; i < MAZE_ROW; i++) maze[i] = new bool[MAZE_COL];

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			boxes[i][j].alphabet = getRandomChar(65, 70);
		}
	}

//	int c = 0;
//	while (c < ROW * COL) {
//		boxes[c / ROW][c % COL]
//		c += 2;
//	}

	resetMaze();
}

void project_cleanup() {
	system("pause");
}

bool FLAG_RUNNING = true;

Selector help() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (boxes[i][j].invisible) {
				continue;
			}
			for (int m = 0; m < ROW; m++) {
				for (int n = 0; n < COL; n++) {
					if (boxes[m][n].invisible) {
						continue;
					}
					if ((i == m && j == n) || boxes[i][j].alphabet != boxes[m][n].alphabet) {
						continue;
					}
					maze[i + PADDING][j + PADDING] = true;
					maze[m + PADDING][n + PADDING] = true;
					Path p = findPath({j + PADDING,i + PADDING}, {n + PADDING,m + PADDING});
					if (p.turns != -1) {
//						MoveCursorTo({static_cast<SHORT>((1 + LINE) * p.corners[0].x + 1), static_cast<SHORT>((1 + PILAR) * p.corners[0].y + 1)});
//						cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << "\033[049m";
//						MoveCursorTo({static_cast<SHORT>((1 + LINE) * p.corners[1].x + 1), static_cast<SHORT>((1 + PILAR) * p.corners[1].y + 2)});
//						cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << "\033[049m";
//						MoveCursorTo({static_cast<SHORT>((1 + LINE) * p.corners[2].x + 1), static_cast<SHORT>((1 + PILAR) * p.corners[2].y + 3)});
//						cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << "\033[049m";
//						Sleep(1000);
						return Selector{{j + PADDING,i + PADDING}, {n + PADDING,m + PADDING}};
					}
					maze[i + PADDING][j + PADDING] = false;
					maze[m + PADDING][n + PADDING] = false;
				}
			}
		}
	}
	return {{-1,-1},{-1,-1}};
}

void match() {
	if (boxes[selector.c1.y - PADDING][selector.c1.x - PADDING].alphabet == boxes[selector.c2.y - PADDING][selector.c2.x - PADDING].alphabet) {
		maze[selector.c1.y][selector.c1.x] = true;
		maze[selector.c2.y][selector.c2.x] = true;
		Path p = findPath(selector.c1, selector.c2);
		if (p.turns != -1) {
			Coord start = selector.c1;
			int t = 0;
			while (!start.isEqual(p.corners[t])) {
				int dx = p.corners[t].x == start.x ? 0 : (p.corners[t].x - start.x) / abs(p.corners[t].x - start.x);
				int dy = p.corners[t].y == start.y ? 0 : (p.corners[t].y - start.y) / abs(p.corners[t].y - start.y);
				start.x += dx;
				start.y += dy;
				if (start.isEqual(p.corners[t]) && t < p.turns) {
					t++;
				}
				MoveCursorTo({static_cast<SHORT>((1 + LINE) * start.x + 1), static_cast<SHORT>((1 + PILAR) * start.y)});
				MoveCursorTo({static_cast<SHORT>((1 + LINE) * start.x + 1), static_cast<SHORT>((1 + PILAR) * start.y + 1)});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << "\033[049m";
				MoveCursorTo({static_cast<SHORT>((1 + LINE) * start.x + 1), static_cast<SHORT>((1 + PILAR) * start.y + 2)});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << "\033[049m";
				MoveCursorTo({static_cast<SHORT>((1 + LINE) * start.x + 1), static_cast<SHORT>((1 + PILAR) * start.y + 3)});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << "\033[049m";
//								Sleep(50);
			}

			Sleep(150);
			boxes[selector.c1.y - PADDING][selector.c1.x - PADDING].invisible = true;
			boxes[selector.c2.y - PADDING][selector.c2.x - PADDING].invisible = true;
		} else {
			Sleep(100);
			maze[selector.c1.y][selector.c1.x] = false;
			maze[selector.c2.y][selector.c2.x] = false;
		}
	}
	selector.c1 = {-1, -1};
	selector.c2 = {-1, -1};
	draw();
}

int main() {
	project_init();

	const char* s1 = "Meow";
	const char* s2 = string(s1).append(" Goofiest").c_str();
	//cout << s2 << "\n";

	SHORT i = 1000;
	while (i-- > 0) {
		MoveCursorTo({static_cast<SHORT>(rand() % 400), static_cast<SHORT>(rand() % 90)});
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rand() % 16);
		if (rand() % 2 == 0) cout << ".";
		else cout << "*";
//		cout << getRandomChar(35, 95);
//		printf("\033[38;2;%d;%d;%dm%c\033[39m\033[49m", rand() % 256, rand() % 256, rand() % 256,
//			   getRandomChar(25, 95));
		cout << "\033[39m\033[49m";
	}

	draw();
	unsigned char ch;
	while (FLAG_RUNNING) {
		ch = getch();
		switch (ch) {
			case ARROW_UP:
			case 'w':
				if (cursor.y > 0) {
					cursor.y--;
					if (cursor.y == 0) cursor.y = ROW + PADDING - 1;
					draw();
				}
				break;
			case ARROW_LEFT:
			case 'a':
				if (cursor.x > 0) {
					cursor.x--;
					if (cursor.x == 0) cursor.x = COL + PADDING - 1;
					draw();
				}
				break;
			case ARROW_RIGHT:
			case 'd':
				if (cursor.x < COL + PADDING) {
					cursor.x++;
					if (cursor.x == COL + PADDING) cursor.x = PADDING;
					draw();
				}
				break;
			case ARROW_DOWN:
			case 's':
				if (cursor.y < ROW + PADDING) {
					cursor.y++;
					if (cursor.y == ROW + PADDING) cursor.y = PADDING;
					draw();
				}
				break;
			case 'c':
				selector.c1 = {-1, -1};
				selector.c2 = {-1, -1};
				draw();
				break;
			case 'h':
				selector = help();
				draw();
				Sleep(10);
				if (selector.c1.isEqual({-1,-1})) {
					break;
				}
				match();
				draw();
				break;
			case ENTER_KEY:
				if (maze[cursor.y][cursor.x]) break;
				if (selector.c1.x == -1 && !cursor.isEqual(selector.c2)) {
					selector.c1 = cursor;
				} else if (cursor.isEqual(selector.c1)) {
					selector.c1 = {-1,-1};
				} else if (selector.c2.x == -1) {
					selector.c2 = cursor;
				} else if (cursor.isEqual(selector.c2)) {
					selector.c2 = {-1, -1};
				}
				draw();
				if (selector.c1.x != -1 && selector.c2.x != -1) {
					match();
				}
				break;
			case ESC_KEY:
				FLAG_RUNNING = false;
				break;
		}
	}
	project_cleanup();
	return 0;
}