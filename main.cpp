#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cert-msc50-cpp"

#include "main.h"

using namespace std;

int BOARD_START_X;
int BOARD_START_Y;

const int COL = 10;
const int ROW = 8;

const int LINE = 7;
const int PILAR = 3;

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
	Coord add(int dx, int dy);
	Coord add(Coord c);
};

bool Coord::isEqual(Coord c) const {
	return c.x == x && c.y == y;
}

Coord Coord::add(int dx = 0, int dy = 0) {
	x += dx;
	y += dy;
	return {x, y};
}

Coord Coord::add(Coord c) {
	return add(c.x, c.y);
}

Coord cursor = {PADDING, PADDING};

struct Selector {
	Coord c1;
	Coord c2;
};

Selector selector{{-1, -1}, {-1, -1}};

const char* ANSI_RESET_BACKGROUND = "\033[49m";
const char* ANSI_RESET_FOREGROUND = "\033[39m";
const char* ANSI_RESET = "\033[0m";

const char* NORMAL_ANSI = "\033[38;2;255;255;255m\033[49m";
const char* SELECT_COLOR_ANSI = "\033[48;2;123;23;131m";
const char* CURSOR_COLOR_ANSI = "\033[48;2;255;255;255m\x1B[38;2;0;0;0m";

void drawLine(ostringstream &sstr) {
	sstr << str_repeat("─", LINE);
}

void drawNormalBoxContent(ostringstream &sstr, char c = ' ') {
	if (c == ' ') sstr << string(LINE, ' ');
	else sstr << string(LINE / 2, ' ') << c << string((LINE - 1) / 2, ' ');
}

void drawPilar(ostringstream &sstr) {
	sstr << "│";
}

void MoveCursorTo(Coord coord) {
	if (coord.x > SCREEN_WIDTH || coord.y > SCREEN_HEIGHT) return;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdout, {static_cast<SHORT>(coord.x), static_cast<SHORT>(coord.y)});
}

struct BoxPart{
	const char* top_left;
	const char* top_mid;
	const char* top_right;

	const char* mid_left;
	const char* mid_right;

	const char* bot_left;
	const char* bot_mid;
	const char* bot_right;
};

void drawRectangle(const BoxPart& parts, Coord coord, int width, int height, PikaRGB bgColor = {-1,-1,-1}) {
	MoveCursorTo(coord);
	string s = parts.top_left;
	string space;
	string bgColorCode;
	if (bgColor.r != -1) {
		bgColorCode = getBGAnsiCode(bgColor);
	}
	for (int i = 0; i < width; i++) {
		s += parts.top_mid;
	}
	cout << ANSI_RESET_BACKGROUND << s << parts.top_right;

	for (int i = 0; i < height / 2; i++) {
		MoveCursorTo({coord.x, coord.y + i + 1});
		cout << ANSI_RESET_BACKGROUND << parts.mid_left << bgColorCode + string(width, ' ') + ANSI_RESET_BACKGROUND + parts.mid_right;
	}
	s = parts.bot_left;
	for (int i = 0; i < width; i++)
		s += parts.bot_mid;
	MoveCursorTo({coord.x, coord.y + height / 2 + 1});
	s += parts.bot_right;
	cout << ANSI_RESET_BACKGROUND << s;
	MoveCursorTo({0,0}); //Resting coord
}

void drawRoundCornerRectangle(Coord coord, int width, int height, PikaRGB bgColor = {-1,-1,-1}, BoxPart part = {"╭","─","╮","│","│","╰","─","╯"}) {
	part.top_left = "╭";
	part.top_right = "╮";
	part.bot_left = "╰";
	part.bot_right = "╯";
	drawRectangle(part, coord, width, height, bgColor);
}

//void drawChoppedCornerRectangle(Coord coord, int width, int height, PikaRGB bgColor = {-1,-1,-1}, BoxPart part = {"╭","─","╮","│","│","╰","─","╯"}) {
//	part.top_left = "╱";
//	part.top_right = "╲";
//	part.bot_left = "╲";
//	part.bot_right = "╱";
//	drawRectangle(part, coord, width, height, bgColor);
//} Look bad

void drawBoxyLineRectangle(Coord coord, int width, int height, PikaRGB bgColor = {-1,-1,-1}) {
	drawRectangle({"▄","▄","▄","█","█","▀","▀","▀"}, coord, width, height, bgColor);
}

void drawHeavyLineRectangle(Coord coord, int width, int height, PikaRGB bgColor = {-1,-1,-1}) {
	drawRectangle({"┏","━","┓","┃","┃","┗","━","┛"}, coord, width, height, bgColor);
}

void drawDoubleLineRectangle(Coord coord, int width, int height, PikaRGB bgColor = {-1,-1,-1}) {
	drawRectangle({"╔","═", "╗", "║", "║", "╚","═","╝"}, coord, width, height, bgColor);
}

void drawDoubleLineHollowRectangle(Coord coord, int width, int height) {
	MoveCursorTo(coord);
	string s = "╔";
	string space;
	for (int i = 0; i < width; i++) {
		s += "═";
	}
	cout << s << "╗";

	for (int i = 0; i < height / 2; i++) {
		MoveCursorTo({coord.x, coord.y + i + 1});
		cout << "║";
		MoveCursorTo({coord.x + width + 1, coord.y + i + 1});
		cout << "║";
	}
	s = "╚";
	for (int i = 0; i < width; i++)
		s += "═";
	MoveCursorTo({coord.x, coord.y + height / 2 + 1});
	s += "╝";
	cout << s;
	MoveCursorTo({0,0}); //Resting coord
}

void drawThinLineRectangle(Coord coord, int width, int height, PikaRGB bgColor = {-1,-1,-1}) {
	drawRectangle({"┌", "─", "┐", "│", "│", "└", "─", "┘"}, coord, width, height, bgColor);
}

void drawAtPos(Coord coord, const string& s) {
	istringstream sstr(s);
	string temp;
	while (!sstr.eof()) {
		getline(sstr, temp, '\n');
		MoveCursorTo(coord);
		cout << ANSI_RESET_BACKGROUND << temp;
		coord.y++;
	}
	MoveCursorTo({0, 0});
}

void drawAtPosAvoidEmptySpace(Coord coord, const string& s) {
	istringstream sstr(s);
	string temp;
	while (!sstr.eof()) {
		int dx = 0;
		getline(sstr, temp, '\n');
		while (temp[0] == ' ') {
			dx++;
			temp.erase(0, 1);
		}

		MoveCursorTo({coord.x + dx, coord.y});
		cout << temp << ANSI_RESET_BACKGROUND << ANSI_RESET_FOREGROUND;
		coord.y++;
	}
}

//void drawAtPosAvoidEmptySpace2(Coord coord, const string& s) {
//	istringstream sstr(s);
//	string temp;
//	while (!sstr.eof()) {
//		int dx = 0;
//		getline(sstr, temp, '\n');
//		int p1 = 0;
//		int p2 = 0;
//		while (p1 != string::npos) {
//			p1 = temp.find(' ', p1 + 1);
//
//
//		}
//		while (getline(sstr, temp, ' ')) {
//		}
//		getline(sstr, temp, '\n');
//		getline(sstr, temp, '\n');
//		while (temp[0] == ' ') {
//			dx++;
//			temp.erase(0, 1);
//		}
//
//		MoveCursorTo({coord.x + dx, coord.y});
//		cout << temp << ANSI_RESET_BACKGROUND << ANSI_RESET_FOREGROUND;
//		coord.y++;
//	}
//}

void drawCursor() {
	MoveCursorTo({BOARD_START_X + (1 + LINE) * cursor.x + 1, BOARD_START_Y + (1 + PILAR) * cursor.y + 1});
	cout << CURSOR_COLOR_ANSI << "       " << ANSI_RESET_BACKGROUND;
	MoveCursorTo({BOARD_START_X + (1 + LINE) * cursor.x + 1, BOARD_START_Y + (1 + PILAR) * cursor.y + 2});
	if (maze[cursor.y][cursor.x]) {
		cout << CURSOR_COLOR_ANSI << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
	} else {
		cout << CURSOR_COLOR_ANSI << string(LINE / 2, ' ') << "\033[1m" << boxes[cursor.y - PADDING][cursor.x - PADDING].alphabet << string((LINE - 1) / 2, ' ') << ANSI_RESET_BACKGROUND;
	}
	MoveCursorTo({BOARD_START_X + (1 + LINE) * cursor.x + 1, BOARD_START_Y + (1 + PILAR) * cursor.y + 3});
	cout << CURSOR_COLOR_ANSI << "       " << ANSI_RESET_BACKGROUND;
}

void drawBackgroundBox(Coord coord) {
//	MoveCursorTo({BOARD_START_X + (1 + LINE) * coord.x + 1, BOARD_START_Y + (1 + PILAR) * coord.y + 1});
//	cout << ANSI_RESET_BACKGROUND << "       " << ANSI_RESET_BACKGROUND;
//	MoveCursorTo({BOARD_START_X + (1 + LINE) * coord.x + 1, BOARD_START_Y + (1 + PILAR) * coord.y + 2});
//	cout << "  XXX   ";
//	MoveCursorTo({BOARD_START_X + (1 + LINE) * coord.x + 1, BOARD_START_Y + (1 + PILAR) * coord.y + 3});
//	cout << ANSI_RESET_BACKGROUND << "       " << ANSI_RESET_BACKGROUND;
}

void drawBox(int i, int j) {
	if (maze[i][j]) return;
	MoveCursorTo({BOARD_START_X + (1 + LINE) * j + 1, BOARD_START_Y + (1 + PILAR) * i + 1});
	cout << NORMAL_ANSI << "       " << ANSI_RESET_BACKGROUND;
	MoveCursorTo({BOARD_START_X + (1 + LINE) * j + 1, BOARD_START_Y + (1 + PILAR) * i + 2});
	cout << NORMAL_ANSI << string(LINE / 2, ' ') << "\033[1m" << boxes[i - PADDING][j - PADDING].alphabet << "\033[0m" << string((LINE - 1) / 2, ' ') << ANSI_RESET_BACKGROUND;
	MoveCursorTo({BOARD_START_X + (1 + LINE) * j + 1, BOARD_START_Y + (1 + PILAR) * i + 3});
	cout << NORMAL_ANSI << "       " << ANSI_RESET_BACKGROUND;
}

void draw() {
//	drawRoundCornerRectangle({BOARD_START_X, BOARD_START_Y}, (LINE + 1) * MAZE_COL, (PILAR + 1) * MAZE_ROW * 2);
//	drawBoxyLineRectangle({BOARD_START_X + LINE + 1, BOARD_START_Y + PILAR + 1}, (LINE - 0.5) *  MAZE_COL + 1, (PILAR) * MAZE_ROW * 2 + 2);
	drawAtPos({BOARD_START_X + 1,BOARD_START_Y + 1}, getFileContent("poké-side-of-the-block.txt"));
	//drawAtPosAvoidEmptySpace({120,9}, getFileContent("poké-side-of-the-block.txt"));
	ostringstream sstr;

	for (int i = 0; i < MAZE_ROW; i++) {
		for (int j = 0; j < MAZE_COL; j++) {
			string bgColor = NORMAL_ANSI;
			if (!maze[i][j]) {
				drawBox(i, j);
			} else {
				drawBackgroundBox({j, i});
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

void project_init() {
	SetConsoleTitleW(L"Pikachu - Matching Game");
	consoleInit();

	boxes = new Box*[ROW];
	for (int i = 0; i < ROW; i++) boxes[i] = new Box[COL];

	maze = new bool*[MAZE_ROW];
	for (int i = 0; i < MAZE_ROW; i++) {
		maze[i] = new bool[MAZE_COL];
		for (int j = 0; j < MAZE_COL; j++) {
			if (i == 0 || i == MAZE_ROW - 1 || j == 0 || j == MAZE_COL -1) {
				maze[i][j] = true;
			} else {
				maze[i][j] = boxes[i - PADDING][j - PADDING].invisible;
			}
		}
	}

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			boxes[i][j].alphabet = getRandomCharInRange(65, 70);
		}
	}
	BOARD_START_X = (SCREEN_WIDTH / 3 * 2) / MAZE_COL;
	BOARD_START_Y = PILAR + 1;
}

void project_cleanup() {
	for (int i = 0; i < MAZE_ROW; i++) {
		if (i != 0 && i != MAZE_ROW - 1) delete[] boxes[i - PADDING];
		delete[] maze[i];
	}
	delete boxes;
	delete maze;
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
			vector<Coord> coords;
			coords.push_back(start);
			int t = 0;
			MoveCursorTo({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 1});
			cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
			MoveCursorTo({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 2});
			cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
			MoveCursorTo({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 3});
			cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
			Sleep(1);
			while (!start.isEqual(p.corners[t])) {
				int dx = p.corners[t].x == start.x ? 0 : (p.corners[t].x - start.x) / abs(p.corners[t].x - start.x);
				int dy = p.corners[t].y == start.y ? 0 : (p.corners[t].y - start.y) / abs(p.corners[t].y - start.y);
				start.x += dx;
				start.y += dy;
				if (start.isEqual(p.corners[t]) && t < p.turns) {
					t++;
				}
				coords.push_back(start);
				MoveCursorTo({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 1});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
				MoveCursorTo({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 2});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
				MoveCursorTo({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 3});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
				Sleep(10);
			}
			Sleep(100);
			draw();
			for (auto & cods : coords) {
				switch (rand() % 37) {
					case 0:
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << ANSI_RESET_BACKGROUND;
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "  :D   " << ANSI_RESET_BACKGROUND;
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << ANSI_RESET_BACKGROUND;
						break;
					case 1:
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "  :>   " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 2:
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "  :O   " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 3:
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ♥   " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 4:
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ★   " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 5:
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ☆   " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 6:
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ♫   " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 7:
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ⛛   " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 8:
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ♜   " << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					default:
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << string(LINE, ' ') << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << string(LINE, ' ') << "\033[049m";
						MoveCursorTo({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << string(LINE, ' ') << "\033[049m";
				}
			}

			Sleep(250);
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

	SHORT i = 1000;
	while (i-- > 0) {
		MoveCursorTo({rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT});
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rand() % 16);
		if (rand() % 2 == 0) cout << ".";
		else cout << "*";
//		cout << getRandomChar(35, 95);
//		printf("\033[38;2;%d;%d;%dm%c\033[39m\033[49m", rand() % 256, rand() % 256, rand() % 256,
//			   getRandomChar(25, 95));
		cout << "\033[39m\033[49m";
	}

	drawRoundCornerRectangle({BOARD_START_X, BOARD_START_Y}, (LINE + 1) * MAZE_COL, (PILAR + 1) * MAZE_ROW * 2);
	draw();
	drawCursor();
	int input;
	while (FLAG_RUNNING) {
		input = getch();
		if (input == PRE_KEY_1 || input == PRE_KEY_2) {
			input = getch();
			draw();
			switch(input) {
				case CTR_ARROW_UP:
					for (int m = cursor.y - 1; m > 0; m--) {
						if (boxes[m - PADDING][cursor.x - PADDING].invisible) continue;
						cursor.y = m;
						break;
					}
					drawCursor();
					break;
				case CTR_ARROW_DOWN:
					for (int m = cursor.y + 1; m < MAZE_ROW - PADDING; m++) {
						if (boxes[m - PADDING][cursor.x - PADDING].invisible) continue;
						cursor.y = m;
						break;
					}
					drawCursor();
					break;
				case CTR_ARROW_LEFT:
					for (int m = cursor.x - 1; m > 0; m--) {
						if (boxes[cursor.y - PADDING][m - PADDING].invisible) continue;
						cursor.x = m;
						break;
					}
					drawCursor();
					break;
				case CTR_ARROW_RIGHT:
					for (int m = cursor.x + 1; m < MAZE_COL - PADDING; m++) {
						if (boxes[cursor.y - PADDING][m - PADDING].invisible) continue;
						cursor.x = m;
						break;
					}
					drawCursor();
					break;
				case ARROW_UP:
					if (cursor.y > 0) {
						cursor.y--;
						if (cursor.y == 0) cursor.y = ROW + PADDING - 1;
						drawCursor();
					}
					break;
				case ARROW_DOWN:
					if (cursor.y < ROW + PADDING) {
						cursor.y++;
						if (cursor.y == ROW + PADDING) cursor.y = PADDING;
						drawCursor();
					}
					break;
				case ARROW_LEFT:
					if (cursor.x > 0) {
						cursor.x--;
						if (cursor.x == 0) cursor.x = COL + PADDING - 1;
						drawCursor();
					}
					break;
				case ARROW_RIGHT:
					if (cursor.x < COL + PADDING) {
						cursor.x++;
						if (cursor.x == COL + PADDING) cursor.x = PADDING;
						drawCursor();
					}
					break;
			}
		} else {
			switch (input) {
				case CTR_ENTER_KEY:
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
				case ESC_KEY:
					FLAG_RUNNING = false;
					break;
			}
		}
	}
	project_cleanup();
	return 0;
}