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

struct Selected {
	Coord c1;
	Coord c2;
	bool valid = false;
};

Selected chosen{{-1, -1}, {-1, -1}, false};

char getRandomChar(int range_a = 65, int range_b = 90) {
	return (char) (range_a + (rand() % (range_b - range_a + 1)));
}

const char* NORMAL_ANSI = "\033[38;2;255;255;255m\033[49m";
const char* SELECT_COLOR_ANSI = "\033[48;2;123;23;131m";
const char* CURSOR_COLOR_ANSI = "\033[48;2;255;255;255m\x1B[38;2;0;0;0m";

string str_repeat(string s, int n) {
	string s1 = s;
	for (int i=1; i<n;i++) s += s1;
	return s;
}

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

string getFileContent(const string &fileName) {
	ifstream file(fileName);
	if (!file.is_open()) return "";
	ostringstream sstr;
	sstr << file.rdbuf();
	file.close();
	return sstr.str();
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
	drawAtPos({18,9}, getFileContent("poké-side-of-the-block.txt"));
	ostringstream sstr;
	MoveCursorTo({0,0});
//	sstr << "S1: " << chosen.c1.x << ":" << chosen.c1.y << "\nS2: " << chosen.c2.x << ":" << chosen.c2.y << "\n";
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
				if ((chosen.c1.x == j && chosen.c1.y == i) || (chosen.c2.x == j && chosen.c2.y == i)) {
					sstr << SELECT_COLOR_ANSI;
				}
				if (k == PILAR / 2) {
					if (i != 0 && i != MAZE_ROW - 1 && j != 0 && j != MAZE_COL - 1) drawNormalBoxContent(sstr, boxes[i - PADDING][j - PADDING].alphabet);
					else drawNormalBoxContent(sstr);
				} else {
					drawNormalBoxContent(sstr);
				}
				if (cursor.isEqual({j, i}) || chosen.c1.isEqual({j, i}) || chosen.c2.isEqual({j, i})) {
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


void drawOld() {
	drawAtPos({18,9}, getFileContent("poké-side-of-the-block.txt"));
	ostringstream sstr;
	MoveCursorTo({0,0});
//	sstr << "S1: " << chosen.c1.x << ":" << chosen.c1.y << "\nS2: " << chosen.c2.x << ":" << chosen.c2.y << "\n";
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
				if ((chosen.c1.x == j && chosen.c1.y == i) || (chosen.c2.x == j && chosen.c2.y == i)) {
					sstr << SELECT_COLOR_ANSI;
				}
				if (k == PILAR / 2) {
					if (i != 0 && i != MAZE_ROW - 1 && j != 0 && j != MAZE_COL - 1) drawNormalBoxContent(sstr, boxes[i - PADDING][j - PADDING].alphabet);
					else drawNormalBoxContent(sstr);
				} else {
					drawNormalBoxContent(sstr);
				}
				if (cursor.isEqual({j, i}) || chosen.c1.isEqual({j, i}) || chosen.c2.isEqual({j, i})) {
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
///////////////////////// NEW BFS

const char DIRECTION[] = "DLRU";

// Arrays to represent change in rows and columns
// DOWN, LEFT, RIGHT, UP
// 0 1 2 3
// 0 - 3 DOWN - UP -> 3 - 0 = 3
// 1 - 2 LEFT - RIGHT -> 3 - 2 = 1

const int dr[4] = {1, 0, 0, -1};
const int dc[4] = {0, -1, 1, 0};

const int MOVE_DOWN = 0;
const int MOVE_LEFT = 1;
const int MOVE_RIGHT = 2;
const int MOVE_UP = 3;
const int MOVE_SIZE = 4;

struct Path{
	Coord corners[2];
	int old_dr;
	int turns = -1;
};

bool isValid(int row, int col) {
	return row >= 0 && col >= 0 && row < MAZE_ROW && col < MAZE_COL;
}

Path findPath(Coord src, Coord dest) {
	if ((src.x == dest.x && abs(src.y - dest.y) == 1) || (src.y == dest.y && abs(src.x - dest.x) == 1)) {
		return Path{{}, -1, 0};
	}
	if (!maze[src.y][src.x] || !maze[dest.y][dest.x]) {
		return Path{{}, -1, 0};
	}
	queue<Path> q;
	q.push({{src}, -1, 0});
	while (!q.empty()) {
		auto cur = q.front();
		Coord coord = cur.corners[cur.turns];
		for (int i = MOVE_DOWN; i <= MOVE_SIZE; i++) {
			if (i == cur.old_dr) {
				continue;
			}
			int row = coord.y + dr[i];
			int col = coord.x + dc[i];

			if (isValid(row, col) && maze[row][col]) {
				int tPoint = cur.turns;
				if (cur.old_dr != -1 && i != cur.old_dr) tPoint++;
				if (tPoint > 2) {
					continue;
				}
				if (tPoint == 2 && (row != dest.y && col != dest.x)) {
					continue;
				}
				cur.corners[tPoint - 1] = {col, row};
				cur.turns = tPoint;
				cur.old_dr = i;
				if (col == dest.x && row == dest.y) {
					return cur;
				}
				q.push(cur);
			}
		}
	}
	return Path{{}, -1};
}

/////////////////////////

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

void printDumpWay(Coord trace[MAZE_ROW][MAZE_COL], int turnMap[MAZE_ROW][MAZE_COL], Coord src, Coord dest) {
	if (!src.isEqual(dest)) {
		printDumpWay(trace, turnMap, src, trace[dest.y][dest.x]);
	}
//	MoveCursorTo({static_cast<SHORT>(LINE * dest.x + LINE + LINE/2), static_cast<SHORT>(dest.y * PILAR + PILAR + PILAR / 2)});
	MoveCursorTo({static_cast<SHORT>((1 + LINE) * dest.x + LINE / 2), static_cast<SHORT>((1 + PILAR) * dest.y + PILAR/2)});
	cout << "X";
}

int getYDirectionFromCoord(Coord a, Coord b) {
	return a.y == b.y ? -1 : 3 * (abs(a.y - b.y) < 0);
}

int getXDirectionFromCoord(Coord a, Coord b) {
	return a.x = b.x ? -1 : 1 + (abs(a.x - b.x) < 0);
}

// Modified BFS Algorithm
int findPath(Coord src, Coord dest, string &path) {
	if ((src.x == dest.x && abs(src.y - dest.y) == 1) || (src.y == dest.y && abs(src.x - dest.x) == 1)) {
		return 0;
	}
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

	int turnMap[MAZE_ROW][MAZE_COL] = {0};
	for (auto & i : turnMap)
		for (int & j : i)
			j = 3;
	turnMap[src.y][src.x] = 0;

	queue<QueueNode> q;
	q.push({src, -1, 0});

	while (!q.empty()) {
		auto cur = q.front();
		Coord coord = cur.coord;
		if (coord.x == dest.x && coord.y == dest.y) {
			MoveCursorTo({0, 50});
			print2DArray(turnMap, src, dest);
			Coord drawer = dest;
			MoveCursorTo({0, 50 + MAZE_ROW + 1});
			cout << string(50, ' ');
			MoveCursorTo({0, 50 + MAZE_ROW + 1});
			int old_d = -1;
//			while (!drawer.isEqual(src)) {
//				for (int i = 0; i < 4; i++) {
//					int row = drawer.y + dr[i];
//					int col = drawer.x + dc[i];
//					if (i == old_d || turnMap[row][col] > turnMap[drawer.y][drawer.x]) continue;
//					drawer = {col, row};
//					old_d = 3 - i;
//					cout << col << ":" << row << " ";
//					break;
//				}
//				cout << "FAILED!";
//				break;
//			}
			return cur.turns;
		}
		q.pop();
		for (int i = 0; i < 4; i++) {
			if (i == 3 - cur.old_dr) {
				continue;
			}
//			if (cur.turns == 1) {
//				if (coord.y == dest.y) {
//					int bX = getXDirectionFromCoord(coord, dest);
//					if (bX != -1 && i != bX) continue;
//				}
//				if (coord.y == dest.x) {
//					int bY = getYDirectionFromCoord(coord, dest);
//					if (bY != -1 && i != bY) continue;
//				}
//			}
			int row = coord.y + dr[i];
			int col = coord.x + dc[i];
			MoveCursorTo({0, 50});
			print2DArray(turnMap, src, dest);

			if (isValid(row, col) && maze[row][col]) {
				int tPoint = cur.turns;
				if (cur.old_dr != -1 && i != cur.old_dr) tPoint++;
				if (tPoint > 2) {
					continue;
				}
				if (tPoint == 2 && (row != dest.y && col != dest.x)) {
					continue;
				}
//				if (tPoint == 2) {
//					if (row == dest.y) {
//						int bX = getXDirectionFromCoord(coord, dest);
//						if (bX != -1 && i != bX) continue;
//					}
//					if (col == dest.x) {
//						int bY = getYDirectionFromCoord(coord, dest);
//						if (bY != -1 && i != bY) continue;
//					}
//				}

				turnMap[row][col] = tPoint;
//				Sleep(1000);
				q.push({{col, row}, i, tPoint});
			}
		}
	}
	MoveCursorTo({0, 50});
	print2DArray(turnMap, src, dest);
	cout << path << "\n";
	return -1;
}

// Modified rat in a maze
int findArcPath(Coord src, Coord dest, vector<string> &ans, string &path, int turn, int skip = -1) {
	if (src.isEqual(dest)) {
		ans.push_back(path);
		return 1;
	}

	for (int i = 0; i < 4; i++) {
		if (i == skip) {
			continue;
		}
		int row = src.y + dr[i];
		int col = src.x + dc[i];

		if (isValid(row, col) && maze[row][col]) {
			int tPoint = turn;
			if (skip != -1 && i != 3 - skip) tPoint++;
			if (tPoint > 2) {
				continue;
			}
			if (tPoint == 2 && (row != dest.y && col != dest.x)) {
				continue;
			}
			path += DIRECTION[i];
			findArcPath({col, row}, dest, ans, path, tPoint, 3 - i);
			path.pop_back();
		}
	}
	return -1;
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
				chosen.c1 = {-1, -1};
				chosen.c2 = {-1, -1};
				draw();
				break;
			case ENTER_KEY:
				if (maze[cursor.y][cursor.x]) break;
				if (chosen.c1.x == -1 && !cursor.isEqual(chosen.c2)) {
					chosen.c1 = cursor;
				} else if (cursor.isEqual(chosen.c1)) {
					chosen.c1 = {-1,-1};
				} else if (chosen.c2.x == -1) {
					chosen.c2 = cursor;
				} else if (cursor.isEqual(chosen.c2)) {
					chosen.c2 = {-1, -1};
				}
				draw();
				if (chosen.c1.x != -1 && chosen.c2.x != -1) {
					if (boxes[chosen.c1.y - PADDING][chosen.c1.x - PADDING].alphabet == boxes[chosen.c2.y - PADDING][chosen.c2.x - PADDING].alphabet) {
						maze[chosen.c1.y][chosen.c1.x] = true;
						maze[chosen.c2.y][chosen.c2.x] = true;
						string tempString;
						if (findPath(chosen.c1, chosen.c2, tempString) != -1) {
							string path;
							vector<string> ans;
							findArcPath(chosen.c1, chosen.c2, ans, path, 0);
							MoveCursorTo({180, 80});
							Coord drawer = chosen.c1;
							int max = 0;
							for (int i = 1; i < size(ans); i++) {
								if (ans[i].length() < ans[max].length()) {
									max = i;
								}
							}
							for (int sS = 0; ans[max][sS]; sS++) {
								switch(ans[max][sS]) {
									case 'D':
										drawer.x += dc[0];
										drawer.y += dr[0];
										break;
									case 'L':
										drawer.x += dc[1];
										drawer.y += dr[1];
										break;
									case 'R':
										drawer.x += dc[2];
										drawer.y += dr[2];
										break;
									case 'U':
										drawer.x += dc[3];
										drawer.y += dr[3];
										break;
								}
//								MoveCursorTo({static_cast<SHORT>((1 + LINE) * drawer.x + LINE / 2), static_cast<SHORT>((1 + PILAR) * drawer.y + PILAR/2)});
								MoveCursorTo({static_cast<SHORT>((1 + LINE) * drawer.x + 1), static_cast<SHORT>((1 + PILAR) * drawer.y)});
//								SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
//								cout << string(LINE, ' ');
//								cout << "\033[48;2;245;25;25m" << string(LINE, ' ') << "\033[049m";
								MoveCursorTo({static_cast<SHORT>((1 + LINE) * drawer.x + 1), static_cast<SHORT>((1 + PILAR) * drawer.y + 1)});
								cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << "\033[049m";
								MoveCursorTo({static_cast<SHORT>((1 + LINE) * drawer.x + 1), static_cast<SHORT>((1 + PILAR) * drawer.y + 2)});
								cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << "\033[049m";
								MoveCursorTo({static_cast<SHORT>((1 + LINE) * drawer.x + 1), static_cast<SHORT>((1 + PILAR) * drawer.y + 3)});
								cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << "\033[049m";
								Sleep(100 / ans[max].length());
							}
							Sleep(150);
							boxes[chosen.c1.y - PADDING][chosen.c1.x - PADDING].invisible = true;
							boxes[chosen.c2.y - PADDING][chosen.c2.x - PADDING].invisible = true;
						} else {
							Sleep(100);
							maze[chosen.c1.y][chosen.c1.x] = false;
							maze[chosen.c2.y][chosen.c2.x] = false;
						}
					}
					chosen.c1 = {-1, -1};
					chosen.c2 = {-1, -1};
					draw();
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