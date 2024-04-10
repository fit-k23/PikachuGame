#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cert-msc50-cpp"

#include "main.h"

using namespace std;

int BOARD_START_X;
int BOARD_START_Y;

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

void drawCursor() {
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * cursor.x + 1, BOARD_START_Y + (1 + PILAR) * cursor.y + 1});
	cout << CURSOR_COLOR_ANSI << "       " << ANSI_RESET_BACKGROUND;
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * cursor.x + 1, BOARD_START_Y + (1 + PILAR) * cursor.y + 2});
	if (maze[cursor.y][cursor.x]) {
		cout << CURSOR_COLOR_ANSI << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
	} else {
		cout << CURSOR_COLOR_ANSI << string(LINE / 2, ' ') << "\033[1m" << boxes[cursor.y - PADDING][cursor.x - PADDING].alphabet << string((LINE - 1) / 2, ' ') << ANSI_RESET_BACKGROUND;
	}
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * cursor.x + 1, BOARD_START_Y + (1 + PILAR) * cursor.y + 3});
	cout << CURSOR_COLOR_ANSI << "       " << ANSI_RESET_BACKGROUND;
}

void drawBackgroundBox(Coord coord) {
//	moveCursorToCoord({BOARD_START_X + (1 + LINE) * coord.x + 1, BOARD_START_Y + (1 + PILAR) * coord.y + 1});
//	cout << ANSI_RESET_BACKGROUND << "       " << ANSI_RESET_BACKGROUND;
//	moveCursorToCoord({BOARD_START_X + (1 + LINE) * coord.x + 1, BOARD_START_Y + (1 + PILAR) * coord.y + 2});
//	cout << "  XXX   ";
//	moveCursorToCoord({BOARD_START_X + (1 + LINE) * coord.x + 1, BOARD_START_Y + (1 + PILAR) * coord.y + 3});
//	cout << ANSI_RESET_BACKGROUND << "       " << ANSI_RESET_BACKGROUND;
}


#include <cassert>

void drawBox(int i, int j) {
	assert(i - PADDING >= 0 && i - PADDING < ROW);
	assert(j - PADDING >= 0 && j - PADDING < COL);
	if (maze[i][j]) return;
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * j + 1, BOARD_START_Y + (1 + PILAR) * i + 1});
	cout << NORMAL_ANSI << "       " << ANSI_RESET_BACKGROUND;
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * j + 1, BOARD_START_Y + (1 + PILAR) * i + 2});
	cout << NORMAL_ANSI << string(LINE / 2, ' ') << "\033[1m" << boxes[i - PADDING][j - PADDING].alphabet << "\033[0m" << string((LINE - 1) / 2, ' ') << ANSI_RESET_BACKGROUND;
	moveCursorToCoord({BOARD_START_X + (1 + LINE) * j + 1, BOARD_START_Y + (1 + PILAR) * i + 3});
	cout << NORMAL_ANSI << "       " << ANSI_RESET_BACKGROUND;
}

void draw() {
//	drawRoundCornerRectangle({BOARD_START_X, BOARD_START_Y}, (LINE + 1) * MAZE_COL, (PILAR + 1) * MAZE_ROW * 2);
//	drawBoxyLineRectangle({BOARD_START_X + LINE + 1, BOARD_START_Y + PILAR + 1}, (LINE - 0.5) *  MAZE_COL + 1, (PILAR) * MAZE_ROW * 2 + 2);
	drawAtPos({BOARD_START_X + 1,BOARD_START_Y + 1}, getFileContent("asset/pikachu_large.txt"));
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

auto musicEngine = AudioEngine();
auto soundEngine = AudioEngine();

void project_init() {
	if (!dirExist(ASSET_RELATIVE_PATH)) {
		cout << getFGAnsiCode(244, 12,21) << "Failed to init project! Project is lack of asset files!\n" << ANSI_RESET;
		return;
	}
	if (!dirExist(MUSIC_RELATIVE_PATH)) {
		cout << getFGAnsiCode(244, 12,21) << "Failed to init project! Project is lack of music files!\n" << ANSI_RESET;
		return;
	}
	if (!dirExist(SOUND_RELATIVE_PATH)) {
		cout << getFGAnsiCode(244, 12,21) << "Failed to init project! Project is lack of sound files!\n" << ANSI_RESET;
		return;
	}
	musicEngine.init();
	soundEngine.init();

	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "Pokemon OR&AS OST Littleroot Town.mp3");
	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "Pokemon OR&AS OST Soaring The Sky (Night).mp3");
	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "AZALI - theme of a shop that sells things you dont want.mp3");
	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "Bitzel - Silly tune.mp3");
	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "is it good enough to be called elevator music.mp3");
	musicEngine.addSoundFromFile(string(MUSIC_RELATIVE_PATH) + "The 4 Corners - Phantom Funk.mp3");
	musicEngine.loop = true;
	musicEngine.volume = 1.0;

	soundEngine.addSoundFromFile(string(SOUND_RELATIVE_PATH) + "button_confirm.mp3");
	soundEngine.addSoundFromFile(string(SOUND_RELATIVE_PATH) + "button_tap.mp3");
	soundEngine.addSoundFromFile(string(SOUND_RELATIVE_PATH) + "correct.wav");
	soundEngine.addSoundFromFile(string(SOUND_RELATIVE_PATH) + "cursor.wav");
	soundEngine.addSoundFromFile(string(SOUND_RELATIVE_PATH) + "select.wav");
	soundEngine.addSoundFromFile(string(SOUND_RELATIVE_PATH) + "wrong.wav");
	soundEngine.volume = 1.0;

	SetConsoleTitleW(L"Pikachu - Matching Game 💀 （╯°□°）╯︵◓");
	consoleInit();

	setBoardSize(8, 10);
//	setBoardSize(4, 5);

	BOARD_START_X = (SCREEN_WIDTH / 3 * 2) / MAZE_COL;
	BOARD_START_Y = PILAR + 1;

//	std::thread musicThread(playLoop, &musicEngine);
//	musicThread.detach();

}

void project_cleanup() {
	uninitBoard();
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
				moveCursorToCoord({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 1});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
				moveCursorToCoord({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 2});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
				moveCursorToCoord({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 3});
				cout << "\033[48;2;245;245;25m" << string(LINE, ' ') << ANSI_RESET_BACKGROUND;
				Sleep(10);
			}
			Sleep(20);
			draw();
			for (auto & cods : coords) {
				switch (rand() % 37) {
					case 0:
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << ANSI_RESET_BACKGROUND;
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "  :D   " << ANSI_RESET_BACKGROUND;
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << ANSI_RESET_BACKGROUND;
						break;
					case 1:
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "  :>   " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 2:
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "  :O   " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 3:
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ♥   " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 4:
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ★   " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 5:
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ☆   " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 6:
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ♫   " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 7:
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ⛛   " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					case 8:
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "   ♜   " << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << "       " << "\033[049m";
						break;
					default:
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 1});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << string(LINE, ' ') << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 2});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << string(LINE, ' ') << "\033[049m";
						moveCursorToCoord({BOARD_START_X + (1 + LINE) * cods.x + 1, BOARD_START_Y + (1 + PILAR) * cods.y + 3});
						cout << getBGAnsiCode(rand() % 256, rand() % 256, rand() % 256) << string(LINE, ' ') << "\033[049m";
				}
			}

			Sleep(100);
			boxes[selector.c1.y - PADDING][selector.c1.x - PADDING].invisible = true;
			boxes[selector.c2.y - PADDING][selector.c2.x - PADDING].invisible = true;
			ma_sound_start(&soundEngine.sounds[2]);
		} else {
			Sleep(50);
			maze[selector.c1.y][selector.c1.x] = false;
			maze[selector.c2.y][selector.c2.x] = false;
			ma_sound_start(&soundEngine.sounds[5]);
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
		moveCursorToCoord({rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT});
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rand() % 16);
		if (rand() % 2 == 0) cout << ".";
		else cout << "*";
//		cout << getRandomChar(35, 95);
//		printf("\033[38;2;%d;%d;%dm%c\033[39m\033[49m", rand() % 256, rand() % 256, rand() % 256,
//			   getRandomChar(25, 95));
		cout << "\033[39m\033[49m";
	}

	drawRoundCornerRectangle({BOARD_START_X, BOARD_START_Y}, (LINE + 1) * MAZE_COL, (PILAR + 1) * MAZE_ROW * 2, {255, 255,0});
	draw();
	drawCursor();
	int input;
	while (FLAG_RUNNING) {
		input = getch();
		if (input == PRE_KEY_1 || input == PRE_KEY_2) {
			input = getch();
			draw();
			ma_sound_start(&soundEngine.sounds[3]);
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
					ma_sound_start(&soundEngine.sounds[4]);
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
					Sleep(1);
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