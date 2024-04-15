#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cert-msc50-cpp"

#include "main.h"

using namespace std;

int BOARD_START_X;
int BOARD_START_Y;

int SCORE_BOARD_X;
int SCORE_BOARD_Y;

AnsiArt bgAnsi;

//#35374B
//#344955
//#50727B
//#78A083
//https://colorhunt.co/palette/35374b34495550727b78a083
//
//#B3C8CF
//#BED7DC
//#F1EEDC
//#E5DDC5
//https://colorhunt.co/palette/b3c8cfbed7dcf1eedce5ddc5
//
//#070F2B
//#1B1A55
//#535C91
//#9290C3
//https://colorhunt.co/palette/070f2b1b1a55535c919290c3

PikaRGB color[12] = {
		{53,  55,  75}, //#35374B
		{52,  73,  85}, //#344955
		{80,  114, 123}, //#50727B
		{120, 160, 131}, //#78A083
		{179, 200, 207}, //#B3C8CF
		{190, 215, 220}, //#BED7DC
		{241, 238, 220}, //#F1EEDC
		{229, 221, 197}, //#E5DDC5
		{7,   15,  43}, //#070F2B
		{27,  26,  85}, //#1B1A55
		{83,  92,  145}, //#535C91
		{146, 144, 195} //#9290C3
};

string bgAn[12] = {
		getBGAnsiCode(color[0]),
		getBGAnsiCode(color[1]),
		getBGAnsiCode(color[2]),
		getBGAnsiCode(color[3]),
		getBGAnsiCode(color[4]),
		getBGAnsiCode(color[5]),
		getBGAnsiCode(color[6]),
		getBGAnsiCode(color[7]),
		getBGAnsiCode(color[8]),
		getBGAnsiCode(color[9]),
		getBGAnsiCode(color[10]),
		getBGAnsiCode(color[11])
};

string fgAn[12] = {
		getFGAnsiCode(getSuitAbleFGColor(color[0])),
		getFGAnsiCode(getSuitAbleFGColor(color[1])),
		getFGAnsiCode(getSuitAbleFGColor(color[2])),
		getFGAnsiCode(getSuitAbleFGColor(color[3])),
		getFGAnsiCode(getSuitAbleFGColor(color[4])),
		getFGAnsiCode(getSuitAbleFGColor(color[5])),
		getFGAnsiCode(getSuitAbleFGColor(color[6])),
		getFGAnsiCode(getSuitAbleFGColor(color[7])),
		getFGAnsiCode(getSuitAbleFGColor(color[8])),
		getFGAnsiCode(getSuitAbleFGColor(color[9])),
		getFGAnsiCode(getSuitAbleFGColor(color[10])),
		getFGAnsiCode(getSuitAbleFGColor(color[11]))
};

void drawCursor() {
	string bg;
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * cursor.x + 1, BOARD_START_Y + (1 + PILAR) * cursor.y + 1});
	cout << bg << "┏";
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * cursor.x + 1 + 6, BOARD_START_Y + (1 + PILAR) * cursor.y + 1});
	cout << bg << "┓";
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * cursor.x + 1, BOARD_START_Y + (1 + PILAR) * cursor.y + 3});
	cout << bg << "┗";
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * cursor.x + 1 + 6, BOARD_START_Y + (1 + PILAR) * cursor.y + 3});
	cout << bg << "┛";
}

void drawSelect(Coord coord) {
	int r = boxes[coord.y - PADDING][coord.x - PADDING].color;
	string bg = bgAn[r];
	string fg = fgAn[r];
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * coord.x + 1, BOARD_START_Y + (1 + PILAR) * coord.y + 1});
	cout << bg << fg << "╭" << ANSI_RESET;
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * coord.x + 1 + 6, BOARD_START_Y + (1 + PILAR) * coord.y + 1});
	cout << bg << fg << "╮" << ANSI_RESET;
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * coord.x + 1, BOARD_START_Y + (1 + PILAR) * coord.y + 3});
	cout << bg << fg << "╰" << ANSI_RESET;
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * coord.x + 1 + 6, BOARD_START_Y + (1 + PILAR) * coord.y + 3});
	cout << bg << fg << "╯" << ANSI_RESET;
}

void drawBackgroundBox(Coord coord) {
	drawAtPos({BOARD_START_X + (1 + LINE) * coord.x + 1, BOARD_START_Y + (1 + PILAR) * coord.y + 1},
			  bgAnsi.frames[1 + (coord.y) * MAZE_COL + coord.x]);
}

void drawBox(int i, int j) {
	if (maze[i][j]) return;
	int r = boxes[i - PADDING][j - PADDING].color;
	string bg = bgAn[r];
	string fg = fgAn[r];
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * j + 1, BOARD_START_Y + (1 + PILAR) * i + 1});
	cout << bg << "       " << ANSI_RESET_BACKGROUND;
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * j + 1, BOARD_START_Y + (1 + PILAR) * i + 2});
//	moveCursorToCoord({BOARD_START_X + (1 + LINE) * j + 1 + LINE / 2, BOARD_START_Y + (1 + PILAR) * i + 2});
//	cout << boxes[i - PADDING][j - PADDING].alphabet;
	cout << bg << string(LINE / 2, ' ') << fg << boxes[i - PADDING][j - PADDING].alphabet << string((LINE - 1) / 2, ' ')
		 << ANSI_RESET_BACKGROUND;
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * j + 1, BOARD_START_Y + (1 + PILAR) * i + 3});
	cout << bg << "       " << ANSI_RESET_BACKGROUND;
}

void draw() {
	for (int i = 0; i < MAZE_ROW; i++) {
		for (int j = 0; j < MAZE_COL; j++) {
			if (selector.c1.isEqual({j, i}) || selector.c2.isEqual({j, i})) {
				drawSelect({j, i});
			} else if (!maze[i][j]) {
				drawBox(i, j);
			} else {
				drawBackgroundBox({j, i});
			}
		}
	}
}

//void drawOld() {
//	//drawAtPos({18,9}, getFileContent("poké-side-of-the-block.txt"));
//	ostringstream sstr;
////	sstr << "S1: " << selector.c1.x << ":" << selector.c1.y << "\nS2: " << selector.c2.x << ":" << selector.c2.y << "\n";
//	for (int i = 0; i < MAZE_ROW; i++) {
//		for (int j = 0; j < MAZE_COL; j++) {
//			sstr << ' ';
//
//			if (i != 0 && i != MAZE_ROW - 1) {
////				if (i == MAZE_ROW - 2 && boxes[i - PADDING - 1][j - PADDING].invisible) drawNormalBoxContent(sstr);
//				if (i == PADDING && boxes[i - PADDING][j - PADDING].invisible) drawNormalBoxContent(sstr);
//				else if (i >= 2 && boxes[i - PADDING][j - PADDING].invisible && boxes[i - PADDING - 1][j - PADDING].invisible) drawNormalBoxContent(sstr);
//				else if (j != 0 && j != MAZE_COL - 1) drawLine(sstr);
//				else drawNormalBoxContent(sstr);
//			} else {
//				if (j != 0 && j != MAZE_COL - 1 && i == MAZE_ROW - 1 && !boxes[i - PADDING - 1][j - PADDING].invisible) drawLine(sstr);
//				else drawNormalBoxContent(sstr);
//			}
//		}
//		sstr << " \n";
//
//		for (int k = 0; k < PILAR; k++) {
//			for (int j = 0; j < MAZE_COL; j++) {
//				if (i != 0 && i != MAZE_ROW - 1 && j != 0) {
//					if (j == MAZE_COL - 1 && boxes[i - PADDING][j - PADDING - 1].invisible) sstr << ' ';
//					else if (j == 1 && boxes[i - PADDING][j - PADDING].invisible) sstr << ' ';
//					else if (j >= 2 && boxes[i - PADDING][j - PADDING].invisible && boxes[i - PADDING][j - PADDING - 1].invisible) sstr << ' ';
//					else drawPilar(sstr);
//				} else {
//					sstr << ' ';
//				}
//				if (i >= 1 && j >= 1 && i <= MAZE_ROW - 2 && j <= MAZE_COL - 2 && boxes[i - PADDING][j - PADDING].invisible) {
//					if (cursor.isEqual({j, i})) {
//						sstr << CURSOR_COLOR_ANSI;
//					}
//					drawNormalBoxContent(sstr);
//					sstr << NORMAL_ANSI;
//					continue;
//				}
////				if (y == i && x == j) {
//				if (cursor.isEqual({j, i})) {
//					sstr << CURSOR_COLOR_ANSI;
//				}
//				if ((selector.c1.x == j && selector.c1.y == i) || (selector.c2.x == j && selector.c2.y == i)) {
//					sstr << SELECT_COLOR_ANSI;
//				}
//				if (k == PILAR / 2) {
//					if (i != 0 && i != MAZE_ROW - 1 && j != 0 && j != MAZE_COL - 1) drawNormalBoxContent(sstr, boxes[i - PADDING][j - PADDING].alphabet);
//					else drawNormalBoxContent(sstr);
//				} else {
//					drawNormalBoxContent(sstr);
//				}
//				if (cursor.isEqual({j, i}) || selector.c1.isEqual({j, i}) || selector.c2.isEqual({j, i})) {
//					sstr << NORMAL_ANSI;
//				}
//			}
//			sstr << " \n";
//		}
//	}
//	cout << sstr.str();
//}

Coord findMatch(Coord src) {
	if (boxes[src.y][src.x].invisible) {
		return {-1, -1};
	}
	for (int m = 0; m < ROW; m++) {
		for (int n = 0; n < COL; n++) {
			if (boxes[m][n].invisible) {
				continue;
			}
			if (src.isEqual({n, m}) || boxes[src.y][src.x].alphabet != boxes[m][n].alphabet) {
				continue;
			}
			maze[src.y + PADDING][src.x + PADDING] = true;
			maze[m + PADDING][n + PADDING] = true;
			Path p = findPath({src.y + PADDING, src.x + PADDING}, {n + PADDING, m + PADDING});
			if (p.turns != -1) {
				if (p.turns > 2) {
					system("start cmd.exe /k echo Hey1");
				}
				return {n + PADDING, m + PADDING};
			} else {
				maze[src.y + PADDING][src.x + PADDING] = false;
				maze[m + PADDING][n + PADDING] = false;
			}
		}
	}
	return {-1, -1};
}

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
					Path p = findPath({j + PADDING, i + PADDING}, {n + PADDING, m + PADDING});
					if (p.turns != -1) {
						return Selector{{j + PADDING, i + PADDING},
										{n + PADDING, m + PADDING}};
					}
					maze[i + PADDING][j + PADDING] = false;
					maze[m + PADDING][n + PADDING] = false;
				}
			}
		}
	}
	return {{-1, -1},
			{-1, -1}};
}

//Help is broken
Selector help2() {
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			if (boxes[i][j].invisible) {
				continue;
			}
			Coord result = findMatch({j, i});
			if (result.x != -1) {
				return {{j + PADDING, i + PADDING}, result};
			}
		}
	}
	return {{-1, -1},
			{-1, -1}};
}

auto musicEngine = AudioEngine();
auto soundEngine = AudioEngine();

auto menuMusic = SoundAlbum();
auto gameMusic = SoundAlbum();

int score = 0;

void match() {
	if (boxes[selector.c1.y - PADDING][selector.c1.x - PADDING].alphabet ==
		boxes[selector.c2.y - PADDING][selector.c2.x - PADDING].alphabet) {
		maze[selector.c1.y][selector.c1.x] = true;
		maze[selector.c2.y][selector.c2.x] = true;
		Path p = findPath(selector.c1, selector.c2);
		if (p.turns != -1) {
			Coord start = selector.c1;
			vector<Coord> coords;
			coords.push_back(start);
			int t = 0;
			moveCursorToCoord({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 1});
			cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
			moveCursorToCoord({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 2});
			cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
			moveCursorToCoord({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 3});
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
				moveCursorToCoord(
						{BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 1});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
				moveCursorToCoord(
						{BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 2});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
				moveCursorToCoord(
						{BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 3});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
				Sleep(10);
			}
			Sleep(20);
			draw();
			for (auto &cods: coords) {
				switch (rand() % 37) {
					case 0:
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       "
							 << ANSI_RESET_BACKGROUND;
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "  :D   "
							 << ANSI_RESET_BACKGROUND;
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       "
							 << ANSI_RESET_BACKGROUND;
						break;
					case 1:
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "  :>   " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 2:
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "  :O   " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 3:
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ♥   " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 4:
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ★   " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 5:
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ☆   " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 6:
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ♫   " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 7:
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ⛛   " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 8:
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ♜   " << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					default:
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << string(LINE, ' ')
							 << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << string(LINE, ' ')
							 << "\033[049m";
						moveCursorToCoord(
								{BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << string(LINE, ' ')
							 << "\033[049m";
				}
			}

			Sleep(100);
			boxes[selector.c1.y - PADDING][selector.c1.x - PADDING].invisible = true;
			boxes[selector.c2.y - PADDING][selector.c2.x - PADDING].invisible = true;
			ma_sound_start(soundEngine.album->sounds[2].sound);
			score += (p.turns + 1) * coords.size();
			remainPair--;
		} else {
			Sleep(50);
			maze[selector.c1.y][selector.c1.x] = false;
			maze[selector.c2.y][selector.c2.x] = false;
			ma_sound_start(soundEngine.album->sounds[5].sound);
		}
	} else {
		ma_sound_start(soundEngine.album->sounds[5].sound);
	}
	selector.reset();
	draw();
	drawCursor();
}

bool FLAG_RUNNING = true;

void project_init() {
	if (!dirExist(ASSET_RELATIVE_PATH)) {
		cout << getFGAnsiCode(244, 12, 21) << "Failed to init project! Project is lack of asset files!\n" << ANSI_RESET;
		return;
	}
	if (!dirExist(MUSIC_RELATIVE_PATH)) {
		cout << getFGAnsiCode(244, 12, 21) << "Failed to init project! Project is lack of music files!\n" << ANSI_RESET;
		return;
	}
	if (!dirExist(SOUND_RELATIVE_PATH)) {
		cout << getFGAnsiCode(244, 12, 21) << "Failed to init project! Project is lack of sound files!\n" << ANSI_RESET;
		return;
	}

	musicEngine.init();
	soundEngine.init();

//	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "Pokemon OR&AS OST Littleroot Town.mp3");
//	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "Pokemon OR&AS OST Soaring The Sky (Night).mp3");
//	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "AZALI - theme of a shop that sells things you dont want.mp3");
//	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "Bitzel - Silly tune.mp3");
//	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "is it good enough to be called elevator music.mp3");
//	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "The 4 Corners - Phantom Funk.mp3");
//	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "[strawberry jams vol. 4] catapillie - Starfruit Supernova (Pillars of Creation Mix).mp3");
//	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "maidens grove (day).mp3");
//	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "Sonic Colors - ＂Planet Wisp＂ Night Version.mp3");
//	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "Touhou 3 - Music #17 - 永遠の満月 ~ Eternal Full Moon.mp3");

//	musicEngine.loop = true;
//	musicEngine.setVolume(0.4);
//	musicEngine.shuffleSound();

	auto gameSound = new SoundAlbum();
	gameSound->addSoundFromFilePath(soundEngine.engine, string(SOUND_RELATIVE_PATH) + "button_confirm.mp3", true);
	gameSound->addSoundFromFilePath(soundEngine.engine, string(SOUND_RELATIVE_PATH) + "button_tap.mp3", true);
	gameSound->addSoundFromFilePath(soundEngine.engine, string(SOUND_RELATIVE_PATH) + "correct.wav", true);
	ma_sound_set_volume(gameSound->sounds[2].sound, 0.5); // reduce the volume by half
	gameSound->addSoundFromFilePath(soundEngine.engine, string(SOUND_RELATIVE_PATH) + "cursor.wav", true);
	gameSound->addSoundFromFilePath(soundEngine.engine, string(SOUND_RELATIVE_PATH) + "select.wav", true);
	gameSound->addSoundFromFilePath(soundEngine.engine, string(SOUND_RELATIVE_PATH) + "wrong.wav", true);
	soundEngine.album = gameSound;

	SetConsoleTitleW(L"Pikachu - Matching Game 💀 （╯°□°）╯︵◓");
	consoleInit();

	setBoardSize(8, 10);
//	setBoardSize(4, 5);

	BOARD_START_X = (SCREEN_WIDTH / 3 * 2) / MAZE_COL;
	BOARD_START_Y = PILAR + 1;
	SCORE_BOARD_X = (SCREEN_WIDTH / 2) + 20;
	SCORE_BOARD_Y = 20;//(BOARD_START_Y + (PILAR + 1) * MAZE_ROW * 2) - 30 / 2;
	SCORE_BOARD_Y = (BOARD_START_Y + (PILAR + 1) * MAZE_ROW) - 30 / 2;

//	std::thread musicThread(playLoop, &musicEngine);
//	musicThread.detach();
}

void project_cleanup() {
	uninitBoard();
	musicEngine.uninit();
	soundEngine.uninit();
	system("pause");
}

void drawScoreBoard() {
	drawTextAtPos({SCORE_BOARD_X + 2, SCORE_BOARD_Y + 2}, "Score: " + to_string(score));
};

int main() {
	project_init();
	readAnsiFile(string(ASSET_RELATIVE_PATH) + "pikachu_large.txt", bgAnsi);

	SHORT i = 1000;
	while (i-- > 0) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rand() % 16);
		drawTextAtPos({rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT}, rand() % 2 == 0 ? "." : "*");
		cout << ANSI_RESET;
	}

	drawRoundCornerRectangle({BOARD_START_X, BOARD_START_Y}, (LINE + 1) * MAZE_COL, (PILAR + 1) * MAZE_ROW * 2);
	drawThinLineRectangle({SCORE_BOARD_X, SCORE_BOARD_Y}, 60, 30);
	draw();
	drawCursor();
	int input;
	while (FLAG_RUNNING) {
		if (remainPair == 0) {
			drawBoxyLineRectangle({BOARD_START_X, BOARD_START_Y}, (LINE + 1) * MAZE_COL, (PILAR + 1) * MAZE_ROW * 2,
								  {255, 255, 0});
			drawAtPos({BOARD_START_X + 1, BOARD_START_Y + 1}, bgAnsi.frames[0]);
			break;
		}
		drawScoreBoard();
		input = getch();
		if (input == PRE_KEY_1 || input == PRE_KEY_2) {
			input = getch();
			ma_sound_start(soundEngine.album->sounds[3].sound);
			draw();
			switch (input) {
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
					ma_sound_start(soundEngine.album->sounds[4].sound);
					if (maze[cursor.y][cursor.x]) {
						selector.reset();
						draw();
						drawCursor();
						continue;
					}
					if (selector.c1.x == -1 && !cursor.isEqual(selector.c2)) {
						selector.c1 = cursor;
					} else if (cursor.isEqual(selector.c1)) {
						selector.c1 = {-1, -1};
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
					selector.reset();
					draw();
					break;
				case 'n':
					musicEngine.FLAG_PLAY_NEXT_SOUND = true;
					break;
				case 'h': {
					auto suggest = help();
					if (suggest.c1.x != -1) {
						drawSelect(suggest.c1);
						drawSelect(suggest.c2);
//						Sleep(1000);
						selector = suggest;
						draw();
						match();
						drawCursor();
					}
					score -= 10;
					break;
				}
				case ESC_KEY: {
					musicEngine.isPlaying = false;
					FLAG_RUNNING = false;
					break;
				}
			}
		}
	}
	project_cleanup();
	return 0;
}