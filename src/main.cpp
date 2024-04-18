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

int score = 0;

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
	drawAtPos({BOARD_START_X + (1 + LINE) * coord.x + 1, BOARD_START_Y + (1 + PILAR) * coord.y + 1},bgAnsi.frames[1 + (coord.y) * MAZE_COL + coord.x]);
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
	cout << bg << string(LINE / 2, ' ') << fg << boxes[i - PADDING][j - PADDING].alphabet << string((LINE - 1) / 2, ' ') << ANSI_RESET_BACKGROUND;
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
						return {{j + PADDING, i + PADDING},{n + PADDING, m + PADDING}};
					}
					maze[i + PADDING][j + PADDING] = false;
					maze[m + PADDING][n + PADDING] = false;
				}
			}
		}
	}
	return {{-1, -1},{-1, -1}};
}

auto soundEngine = AudioEngine();

auto gameSound = SoundAlbum();
auto menuMusic = SoundAlbum();
auto inGameMusic = SoundAlbum();

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
			string pathBG = getBGAnsiCode(245,245,25).append(string(LINE, ' ')).append(ANSI_RESET_BACKGROUND);
			drawRawTextAtPos({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 1}, pathBG);
			drawRawTextAtPos({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 2}, pathBG);
			drawRawTextAtPos({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 3}, pathBG);
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
				drawRawTextAtPos({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 1}, pathBG);
				drawRawTextAtPos({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 2}, pathBG);
				drawRawTextAtPos({BOARD_START_X + (1 + LINE) * start.x + 1, BOARD_START_Y + (1 + PILAR) * start.y + 3}, pathBG);
				Sleep(10 / coords.size());
			}
			draw();
			Sleep(40);
			boxes[selector.c1.y - PADDING][selector.c1.x - PADDING].invisible = true;
			boxes[selector.c2.y - PADDING][selector.c2.x - PADDING].invisible = true;
			soundEngine.playSyncFromAlbum(gameSound, 0);
			score += (p.turns + 1) * coords.size();
			remainPair--;
		} else {
			Sleep(20);
			maze[selector.c1.y][selector.c1.x] = false;
			maze[selector.c2.y][selector.c2.x] = false;
			soundEngine.playSyncFromAlbum(gameSound, 3);
		}
	} else {
		soundEngine.playSyncFromAlbum(gameSound, 3);
	}
	selector.reset();
	draw();
	drawCursor();
}

bool FLAG_RUNNING = true;

bool project_init() {
	SetConsoleTitleW(L"Pikachu - Matching Game 💀 （╯°□°）╯︵◓");
	consoleInit();

	if (!dirExist(ASSET_RELATIVE_PATH)) {
		cout << getFGAnsiCode(244, 12, 21) << "Failed to init project! Project is lack of asset files!\n" << ANSI_RESET;
		return false;
	}
	loadDataFromFile(string(ASSET_RELATIVE_PATH) + "gameData.txt");
	if (!dirExist(MUSIC_RELATIVE_PATH)) {
		cout << getFGAnsiCode(244, 12, 21) << "Failed to init project! Project is lack of music files!\n" << ANSI_RESET;
		return false;
	}
	if (!dirExist(SOUND_RELATIVE_PATH)) {
		cout << getFGAnsiCode(244, 12, 21) << "Failed to init project! Project is lack of sound files!\n" << ANSI_RESET;
		return false;
	}
	if (!soundEngine.init()) {
		cout << getFGAnsiCode(244, 12, 21) << "Failed to init sound engine! IDK what's happened :c!\n" << ANSI_RESET;
		return false;
	}
	loadPokemon();

	vector<string> soundPaths = {
		"correct.wav",
		"cursor.wav",
		"select.wav",
		"wrong.wav"
	};
	for (const auto & soundPath : soundPaths) {
		if (!gameSound.addSoundFromFilePath(soundEngine, string(SOUND_RELATIVE_PATH) + soundPath)) {
			cout << getFGAnsiCode(244, 12, 21) << "Failed to add music \"" << soundPath << "\"" << ANSI_RESET;
			return false;
		}
	}

	vector<string> musicPaths = {
		"Pokemon OR&AS OST Littleroot Town.mp3",
		"Pokemon OR&AS OST Soaring The Sky (Night).mp3",
		"AZALI - theme of a shop that sells things you dont want.mp3",
		"Bitzel - Silly tune.mp3",
		"is it good enough to be called elevator music.mp3",
		"The 4 Corners - Phantom Funk.mp3",
		"maidens grove (day).mp3",
		"Sonic Colors - Planet Wisp.mp3",
		"Touhou 3 - Eternal Full Moon.mp3"
	};
	for (const auto & musicPath : musicPaths) {
		if (!menuMusic.addSoundFromFilePath(soundEngine, string(MUSIC_RELATIVE_PATH) + musicPath)) {
			cout << getFGAnsiCode(244, 12, 21) << "Failed to add music \"" << musicPath << "\"" << ANSI_RESET;
			return false;
		}
	}

	menuMusic.shuffleSound();
	menuMusic.loop = true;
	menuMusic.randomize = true;

	soundEngine.album = &menuMusic;
	soundEngine.setVolume(1.0);

	thread musicThread(playEngine, &soundEngine);
	musicThread.detach();
	return true;
}

void project_cleanup() {
	saveDataToFile(string(ASSET_RELATIVE_PATH) + "gameData.txt");
	uninitPokemon();
	uninitBoard();
	soundEngine.uninit();
//	system("pause");
}

void drawScoreBoard() {
	int offset = 10;
	string bg = getFGAnsiCode(255,255,255);
	drawRawTextAtPos({SCORE_BOARD_X + 2 + offset, SCORE_BOARD_Y + 2}, bg + "User name: " + userList[userId].userName);
	drawRawTextAtPos({SCORE_BOARD_X + 2 + offset, SCORE_BOARD_Y + 3}, "Pokemon: " + pokemonList.pokemons[userList[userId].pokeId].name);
	drawRawTextAtPos({SCORE_BOARD_X + 2 + offset, SCORE_BOARD_Y + 4}, "Score: " + to_string(score));
//	drawRawTextAtPos({SCORE_BOARD_X + 2, SCORE_BOARD_Y + 5}, (string("Mode: ") + (mode == 0 ? "Normal" : "Collapse")));
	drawRawTextAtPos({SCORE_BOARD_X + 2 + offset, SCORE_BOARD_Y + 5}, "Record: " + to_string(userList[userId].score[mode]));
	drawRawTextAtPos({SCORE_BOARD_X + 2 + offset, SCORE_BOARD_Y + 6}, "Suggestion Left: " + to_string(suggestionTry));
	drawAtPos({SCORE_BOARD_X + 30, SCORE_BOARD_Y + 4}, pokemonList.pokemons[userList[userId].pokeId].art.frames[0]);
}

void makeNewGame(int selectedButtonId) {
	setBoardSize(8, 10, true);

	BOARD_START_X = (SCREEN_WIDTH / 3 * 2) / MAZE_COL;
	BOARD_START_Y = PILAR + 1;
	SCORE_BOARD_X = (SCREEN_WIDTH / 2) + 20;
	SCORE_BOARD_Y = (BOARD_START_Y + (PILAR + 1) * MAZE_ROW) - 30 / 2;


	if (selectedButtonId == CHOOSE_GAME_BUTTON_NORMAL) {
		mode = 0;
	} else if (selectedButtonId == CHOOSE_GAME_BUTTON_COLLAPSE) {
		mode = 1;
	}
	userList[userId].mode = mode;
}

void runGame() {
	fillConsoleBackground(RGB(0, 0, 0));

	readAnsiFile(string(ASSET_RELATIVE_PATH) + "pikachu_large.txt", bgAnsi);

	SHORT i = 300;
	while (i --> 0) {
		drawRawTextAtPos({rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT}, getFGAnsiCode(rand() % 256, rand() % 256, rand() % 256).append(rand() % 2 == 0 ? "." : "*"));
		drawRawTextAtPos({rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT}, getFGAnsiCode(rand() % 256, rand() % 256, rand() % 256).append(rand() % 2 == 0 ? "▪" : "▴"));
		drawRawTextAtPos({rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT}, getFGAnsiCode(rand() % 256, rand() % 256, rand() % 256).append(rand() % 2 == 0 ? "●" : "◌"));
		cout << ANSI_RESET;
	}

	drawRoundCornerRectangle({BOARD_START_X, BOARD_START_Y}, (LINE + 1) * MAZE_COL, ((PILAR + 1) * MAZE_ROW + 2) * 2);

	int offset = 5;
	drawThinLineRectangle({SCORE_BOARD_X + offset, BOARD_START_Y}, 60 - 2*offset, (SCORE_BOARD_Y - BOARD_START_Y) * 2);
	drawThinLineRectangle({SCORE_BOARD_X + offset, SCORE_BOARD_Y}, 60 - 2*offset, 30);
	draw();
	drawCursor();
	int input;
	while (FLAG_RUNNING) {
		if (remainPair == 0) {
			drawBoxyLineRectangle({BOARD_START_X, BOARD_START_Y}, (LINE + 1) * MAZE_COL, (PILAR + 1) * MAZE_ROW * 2,{255, 255, 0});
			drawAtPos({BOARD_START_X + 1, BOARD_START_Y + 1}, bgAnsi.frames[0]);
			break;
		}
		drawScoreBoard();
		input = getch();
		if (input == PRE_KEY_1 || input == PRE_KEY_2) {
			input = getch();
			soundEngine.playSyncFromAlbum(gameSound, 1);
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
					soundEngine.playSyncFromAlbum(gameSound, 2);
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
					soundEngine.FLAG_PLAY_NEXT_SOUND = true;
					break;
				case 'h': {
					if (suggestionTry > 0) {
						auto suggest = help();
						if (suggest.c1.x != -1) {
							drawSelect(suggest.c1);
							drawSelect(suggest.c2);
							selector = suggest;
							draw();
							match();
							drawCursor();
						}
						score -= 10;
						suggestionTry--;
					}
					break;
				}
				case 'q':
				case ESC_KEY: {
					FLAG_RUNNING = false;
					break;
				}
			}
		}
	}
	if (FLAG_RUNNING) {
		userList[userId].score[mode] = score;
		userList[userId].mode = -1;
		userList[userId].lastScore = 0;
		userList[userId].stage = "";
		userList[userId].remainPair = 0;
		userList[userId].suggestionTry = 0;
	}
}

void drawMainMenu(int selectedButtonId) {
	drawAtPos({SCREEN_WIDTH - 160 - 2, 2}, getFileContent(string(TEXT_RELATIVE_PATH) + "pikamatch_text.txt"));

	PikaRGB button1Color = {255,255,255};
	string fg;
	string choseColor = getFGAnsiCode(185, 148, 112);

	int button_width = 56;
	int button_anchor_x = SCREEN_WIDTH / 2 + button_width - 28 + 1 - 20;
	int button_anchor_y = static_cast<int>(SCREEN_HEIGHT / 1.5);

	Coord pokemon_art_pos = {SCREEN_WIDTH - button_anchor_x + 10 - 40, button_anchor_y};
	drawAtPos(pokemon_art_pos, pokemonList.pokemons[userList[userId].pokeId].art.frames[0]);

	if (userList[userId].mode != -1) {
		if (selectedButtonId == MAIN_MENU_BUTTON_CONTINUE_GAME) {
			button1Color = {185, 148, 112};
			fg = choseColor;
		}
		drawRoundCornerRectangle({button_anchor_x, static_cast<int>(button_anchor_y - 8)}, button_width, 8, button1Color);
		drawAtPos({button_anchor_x + 1 + 3, static_cast<int>(button_anchor_y - 8 + 1)}, fg + BUTTON_CONTINUE_GAME);
	}

	PikaRGB button2Color = {255,255,255};
	fg = "";
	if (selectedButtonId == MAIN_MENU_BUTTON_START_GAME) {
		button2Color = {185, 148, 112};
		fg = choseColor;
	}
	drawRoundCornerRectangle({button_anchor_x, static_cast<int>(button_anchor_y - 4)}, button_width, 8, button2Color);
	drawAtPos({button_anchor_x + 1 + 9, static_cast<int>(button_anchor_y - 4 + 1)}, fg + BUTTON_START_GAME);

	PikaRGB button3Color = {255,255,255};
	fg = "";
	if (selectedButtonId == MAIN_MENU_BUTTON_LEADER_BOARD) {
		button3Color = {185, 148, 112};
		fg = choseColor;
	}
	drawRoundCornerRectangle({button_anchor_x, static_cast<int>(button_anchor_y)}, button_width, 8, button3Color);
	drawAtPos({button_anchor_x + 1 + 6, static_cast<int>(button_anchor_y + 1)}, fg + BUTTON_LEADERBOARD);

	PikaRGB button4Color = {255,255,255};
	fg = "";
	if (selectedButtonId == MAIN_MENU_BUTTON_SETTING) {
		button4Color = {185, 148, 112};
		fg = choseColor;
	}
	drawRoundCornerRectangle({button_anchor_x, static_cast<int>(button_anchor_y + 4)}, button_width, 8, button4Color);
	drawAtPos({button_anchor_x + 1 + 6, static_cast<int>(button_anchor_y + 4 + 1)}, fg + BUTTON_SETTING);

	PikaRGB button5Color = {255,255,255};
	fg = "";
	if (selectedButtonId == MAIN_MENU_BUTTON_LOG_OUT) {
		button5Color = {185, 148, 112};
		fg = choseColor;
	}
	drawRoundCornerRectangle({button_anchor_x, static_cast<int>(button_anchor_y + 8)}, button_width, 8, button5Color);
	drawAtPos({button_anchor_x + 1, static_cast<int>(button_anchor_y + 8 + 1)}, fg + BUTTON_SAVE_AND_LOGOUT);
	moveCursorToCoord({SCREEN_WIDTH - 3, SCREEN_HEIGHT - 3});
}

void drawChoseGameMenu(int selectedButtonId) {
	int game_title_width = 160;
	drawAtPos({SCREEN_WIDTH - game_title_width - 2, 2}, getFileContent(string(TEXT_RELATIVE_PATH) + "pikamatch_text.txt"));
	PikaRGB button1Color = {255,255,255};
	int button_anchor_x = SCREEN_WIDTH / 2 + 56 + 26 - 28 + 1 - 20;
	int button_anchor_y = static_cast<int>(SCREEN_HEIGHT / 1.5) - 4;
	int box_width = button_anchor_x - SCREEN_WIDTH + game_title_width - 20;
	drawRoundCornerRectangle({SCREEN_WIDTH - game_title_width - 2, button_anchor_y - 12}, box_width, 48 + 8);
	if (selectedButtonId == CHOOSE_GAME_BUTTON_NORMAL) {
		button1Color = {185, 148, 112};
		drawRawTextAtPos({SCREEN_WIDTH - game_title_width - 2 + 1, button_anchor_y - 12 + 1}, "NORMAL");
	}
	drawRoundCornerRectangle({button_anchor_x, static_cast<int>(button_anchor_y - 4)}, 30, 8, button1Color);
	drawAtPos({button_anchor_x + 1 + 2, static_cast<int>(button_anchor_y - 4 + 1)}, BUTTON_MODE_NORMAL);
	PikaRGB button2Color = {255,255,255};
	if (selectedButtonId == CHOOSE_GAME_BUTTON_COLLAPSE) {
		button2Color = {185, 148, 112};
		drawRawTextAtPos({SCREEN_WIDTH - 160 - 2 + 1, button_anchor_y - 12 + 1}, "COLLAPSE");
	}
	drawRoundCornerRectangle({button_anchor_x, static_cast<int>(button_anchor_y)}, 30, 8, button2Color);
	drawAtPos({button_anchor_x + 1, static_cast<int>(button_anchor_y + 1)}, BUTTON_MODE_COLLAPSE);
	PikaRGB button3Color = {255,255,255};
	if (selectedButtonId == CHOOSE_GAME_BUTTON_RETURN) {
		button3Color = {185, 148, 112};
	}
	drawRoundCornerRectangle({button_anchor_x, static_cast<int>(button_anchor_y + 4)}, 30, 8, button3Color);
	drawAtPos({button_anchor_x + 1 + 3, static_cast<int>(button_anchor_y + 4 + 1)}, BUTTON_RETURN);
	moveCursorToCoord({SCREEN_WIDTH - 3, SCREEN_HEIGHT - 3});
}

void drawLeaderBoard(int selectedButtonId) {
	drawAtPos({12, 10}, getFileContent(string(TEXT_RELATIVE_PATH) + "leaderboard_logo.txt"));

	int leaderboard_text_width = 40;
	drawAtPos({SCREEN_WIDTH / 2 - leaderboard_text_width, 1}, getFileContent(string(TEXT_RELATIVE_PATH) + "leaderboard_text.txt"));
	int a[10];
	getTopRank(a, selectedButtonId);
	string bg1 = getBGAnsiCode(201,166,0);
	string bg2 = getBGAnsiCode(245,245,245);
	string fg1 = getFGAnsiCode(255, 255,255);
	string fg2 = getFGAnsiCode(0,0,0);

	string empty_line = string(70, ' ');
	for (int i = 0; i < 10; i++) {
		string bg = i % 2 == 0 ? bg1 : bg2;
		string fg = i % 2 == 0 ? fg1 : fg2;
		if (a[i] == -1 || selectedButtonId == LEADERBOARD_BUTTON_RETURN) {
			drawAtPos({static_cast<int>(SCREEN_WIDTH / 1.75), 0 + i * 3 + 15}, bg + empty_line);
			drawAtPos({static_cast<int>(SCREEN_WIDTH / 1.75), 1 + i * 3 + 15}, bg + empty_line);
		} else {
			drawAtPos({static_cast<int>(SCREEN_WIDTH / 1.75), 0 + i * 3 + 15}, bg + empty_line);
			if (i < 3 && i >= 0) {
				drawAtPos({static_cast<int>(SCREEN_WIDTH / 1.75) + 60, 0 + i * 3 + 15}, bg + fg + "✮ Top " + to_string(i + 1));
			}
			string name = userList[a[i]].userName;
			drawAtPos({static_cast<int>(SCREEN_WIDTH / 1.75), 0 + i * 3 + 15},  bg + fg + " " + string(20, '_').append(name));
			string scoreStr = to_string(userList[a[i]].score[selectedButtonId]);
//			drawAtPos({static_cast<int>(SCREEN_WIDTH / 1.75), 1 + i * 3 + 15}, bg + fg + string(70 - score.length(), ' ') + score);
			drawAtPos({static_cast<int>(SCREEN_WIDTH / 1.75), 1 + i * 3 + 15}, bg + empty_line);
			drawAtPos({static_cast<int>(SCREEN_WIDTH / 1.75), 1 + i * 3 + 15}, bg + fg + string(50 - scoreStr.length(), ' ').append(scoreStr).append(string(20, '_')));
		}
	}
	PikaRGB button1Color = {255,255,255};
	if (selectedButtonId == CHOOSE_GAME_BUTTON_NORMAL) {
		button1Color = {185, 148, 112};
	}

	int button_anchor_y = static_cast<int>(SCREEN_HEIGHT / 1.35);
	drawRoundCornerRectangle({42 - 7, static_cast<int>(button_anchor_y - 4)}, 30, 8, button1Color);
	drawAtPos({42 - 7 + 3, static_cast<int>(button_anchor_y - 4 + 1)}, BUTTON_MODE_NORMAL);
	string bg_fgColor = getFGAnsiCode(0,0,0);
	PikaRGB button2Color = {255,255,255};
	if (selectedButtonId == CHOOSE_GAME_BUTTON_COLLAPSE) {
		button2Color = {185, 148, 112};
	}
	drawRoundCornerRectangle({42 - 7, static_cast<int>(button_anchor_y)}, 30, 8, button2Color);
	drawAtPos({42 - 7 + 1, static_cast<int>(button_anchor_y + 1)}, BUTTON_MODE_COLLAPSE);
	PikaRGB button3Color = {255,255,255};
	if (selectedButtonId == CHOOSE_GAME_BUTTON_RETURN) {
		button3Color = {185, 148, 112};
	}
	drawRoundCornerRectangle({42 - 7, static_cast<int>(button_anchor_y + 4)}, 30, 8, button3Color);
	drawAtPos({42 - 7 + 4, static_cast<int>(button_anchor_y + 4 + 1)}, BUTTON_RETURN);
	moveCursorToCoord({SCREEN_WIDTH - 3, SCREEN_HEIGHT - 3});
}

void runMainMenu();

void runLoginMenu() {
	fillConsoleBackground(RGB(20, 0, 20));

//	fillConsoleBackground(RGB(229, 205, 174));

	const int LOGIN_BOX_HEIGHT = 22;
	int LOGIN_BOX_WIDTH = SCREEN_WIDTH / 3;

	AnsiArt pokeballs_idle{{
		getFileContent(string(TEXT_RELATIVE_PATH) + "pokeball_default.txt"),
		getFileContent(string(TEXT_RELATIVE_PATH) + "pokeball_left.txt"),
		getFileContent(string(TEXT_RELATIVE_PATH) + "pokeball_default.txt"),
		getFileContent(string(TEXT_RELATIVE_PATH) + "pokeball_right.txt")
   }, 100, true};

	AnsiArt pokeballs_succeed{{
		getFileContent(string(TEXT_RELATIVE_PATH) + "pokeball_success_0.txt"),
		getFileContent(string(TEXT_RELATIVE_PATH) + "pokeball_success_1.txt"),
		getFileContent(string(TEXT_RELATIVE_PATH) + "pokeball_success_2.txt")
	}, 100, false, false};

//	char userName[CHAR_USER_NAME_SIZE] = {};
//	char userPass[CHAR_USER_PASS_SIZE] = {};
	char userName[CHAR_USER_NAME_SIZE] = "Tai1906";
	char userPass[CHAR_USER_PASS_SIZE] = "123";

	cout << getBGAnsiCode(0,0,0);
	drawRoundCornerHollowRectangle({SCREEN_WIDTH / 3, static_cast<int>(SCREEN_HEIGHT / 1.75)}, LOGIN_BOX_WIDTH, LOGIN_BOX_HEIGHT, {255,255,255});

	int loginButtonId = LOGIN_MENU_INPUT_USERNAME;
	bool entered = false;
	bool updated = true;

	int loginState = -2;

	thread loginControlThread(loginKeyboardController, userName, userPass, &loginButtonId, &entered, &loginState, &updated);
	loginControlThread.detach();

	string bg_fgColor = getFGAnsiCode(255,255,255);
	drawRawTextAtPos({SCREEN_WIDTH / 3 + 5, static_cast<int>(SCREEN_HEIGHT / 1.75) + 7}, bg_fgColor + "TIPS: ");
	drawRawTextAtPos({SCREEN_WIDTH / 3 + 10, static_cast<int>(SCREEN_HEIGHT / 1.75) + 7}, bg_fgColor + R"(- Press "ENTER" to confirm!)");
	drawRawTextAtPos({SCREEN_WIDTH / 3 + 10, static_cast<int>(SCREEN_HEIGHT / 1.75) + 8}, bg_fgColor + "- Use arrow keys (▲/▼/</>) for navigating!");
//	drawRawTextAtPos({SCREEN_WIDTH / 3 + 10, static_cast<int>(SCREEN_HEIGHT / 1.75) + 8}, bg_fgColor + "- Use arrow keys (▴/▾/◂/▸) for navigating!");
	drawRawTextAtPos({SCREEN_WIDTH / 3 + 10, static_cast<int>(SCREEN_HEIGHT / 1.75) + 9}, bg_fgColor + R"(- Press "DELETE" or "←BACKSPACE" to delete.)");
	moveCursorToCoord({0,0}); // resting point

	while (true) {
		if (!entered) {
			drawAnsiArt(pokeballs_idle, {SCREEN_WIDTH / 2 - 23, SCREEN_HEIGHT / 4 - 15});
		} else {
			if (loginState == -2) {
				break;
			}
			if (pokeballs_succeed.isDone) {
				if (loginButtonId == LOGIN_MENU_INPUT_SIGNIN) {
					userId = loginState;
				} else {
					userId = login(string(userName), string(userPass));
				}
				clrScr();
				task = TASK_MAIN_MENU;
				break;
			} else {
				drawAnsiArt(pokeballs_succeed, {SCREEN_WIDTH / 2 - 23, SCREEN_HEIGHT / 4 - 15});
			}
		}

		if (updated) {
			moveCursorToCoord({SCREEN_WIDTH / 3 + 7, static_cast<int>(SCREEN_HEIGHT / 1.75) + 2});
			if (loginButtonId == LOGIN_MENU_INPUT_USERNAME) {
				cout << bg_fgColor << " > ";
			} else {
				cout << bg_fgColor << "   ";
			}
			moveCursorToCoord({SCREEN_WIDTH / 3 + 7, static_cast<int>(SCREEN_HEIGHT / 1.75) + 3});
			if (loginButtonId == LOGIN_MENU_INPUT_USERPASS) {
				cout << bg_fgColor << " > ";
			} else {
				cout << bg_fgColor << "   ";
			}

			if (loginState == -1) {
				if (loginButtonId != LOGIN_MENU_INPUT_SIGNIN) {
					drawRawTextAtPos({SCREEN_WIDTH / 3 + 10 + 12 + CHAR_USER_NAME_SIZE, static_cast<int>(SCREEN_HEIGHT / 1.75) + 2}, string(15, ' '));
					drawRawTextAtPos({SCREEN_WIDTH / 3 + 10 + 12 + CHAR_USER_NAME_SIZE, static_cast<int>(SCREEN_HEIGHT / 1.75) + 2}, bg_fgColor + getFGAnsiCode(234, 12, 12) + "User not found!" + ANSI_RESET);
				} else {
					drawRawTextAtPos({SCREEN_WIDTH / 3 + 10 + 12 + CHAR_USER_NAME_SIZE, static_cast<int>(SCREEN_HEIGHT / 1.75) + 2}, string(15, ' '));
					drawRawTextAtPos({SCREEN_WIDTH / 3 + 10 + 12 + CHAR_USER_NAME_SIZE, static_cast<int>(SCREEN_HEIGHT / 1.75) + 2}, bg_fgColor + getFGAnsiCode(234, 12, 12) + "User existed!" + ANSI_RESET);
				}
				soundEngine.playSyncFromAlbum(gameSound, 3);
			} else {
				drawRawTextAtPos({SCREEN_WIDTH / 3 + 10 + 12 + CHAR_USER_NAME_SIZE, static_cast<int>(SCREEN_HEIGHT / 1.75) + 2}, string(15, ' '));
			}

			if (loginState == 0 && loginButtonId != LOGIN_MENU_INPUT_SIGNIN) {
				soundEngine.playSyncFromAlbum(gameSound, 3);
				drawRawTextAtPos({SCREEN_WIDTH / 3 + 10 + 12 + CHAR_USER_PASS_SIZE, static_cast<int>(SCREEN_HEIGHT / 1.75) + 3}, bg_fgColor + getFGAnsiCode(234, 12, 12) + "Wrong Password" + ANSI_RESET);
			} else {
				drawRawTextAtPos({SCREEN_WIDTH / 3 + 10 + 12 + CHAR_USER_PASS_SIZE, static_cast<int>(SCREEN_HEIGHT / 1.75) + 3}, string(15, ' '));
			}

			string buttonColor;
			if (loginButtonId == LOGIN_MENU_INPUT_LOGIN) {
				buttonColor = getFGAnsiCode(185, 148, 112);
			}
			drawRawTextAtPos({SCREEN_WIDTH / 3 + 10, static_cast<int>(SCREEN_HEIGHT / 1.75) + 5}, bg_fgColor + buttonColor + "LOGIN" + ANSI_RESET);

			buttonColor = "";
			if (loginButtonId == LOGIN_MENU_INPUT_SIGNIN) {
				buttonColor = getFGAnsiCode(185, 148, 112);
			}

			drawRawTextAtPos({SCREEN_WIDTH / 3 + 10 + 10 + CHAR_USER_NAME_SIZE - 6, static_cast<int>(SCREEN_HEIGHT / 1.75) + 5}, bg_fgColor + buttonColor + "SIGNUP" + ANSI_RESET);

			drawRawTextAtPos({SCREEN_WIDTH / 3 + 10, static_cast<int>(SCREEN_HEIGHT / 1.75) + 2}, bg_fgColor + "User name: " + userName + string(CHAR_USER_NAME_SIZE - strlen(userName) - 1, '_'));
			drawRawTextAtPos({SCREEN_WIDTH / 3 + 10, static_cast<int>(SCREEN_HEIGHT / 1.75) + 3}, bg_fgColor + "Password:  " + string(strlen(userPass), '*') + string(CHAR_USER_PASS_SIZE - strlen(userPass) - 1, '_'));
			moveCursorToCoord({SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10}); //Resting point! Stop a buggy bug where the cursor break the ansi escape code!

			updated = false;
		}

		if (!entered) {
			Sleep(pokeballs_idle.sleepTime);
		} else {
			if (!pokeballs_succeed.isDone) Sleep(pokeballs_succeed.sleepTime);
		}
	}
}

int drawChoosePokemonMenu() {
	clrScr();
	bool entered = false;
	bool updated = true;
	int pokeId = 0;
	thread choosePokemon(choosePokemonKeyboardController, &pokeId, &updated, &entered);
	choosePokemon.detach();
	PikaRGB buttonColor;
	drawAtPos({SCREEN_WIDTH / 2 - 54, 2}, "▒█▀▀█ ▒█░▒█ ▒█▀▀▀█ ▒█▀▀▀█ ▒█▀▀▀█ ▒█▀▀▀   ▒█░░▒█ ▒█▀▀▀█ ▒█░▒█ ▒█▀▀█   ▒█▀▀█ ▒█▀▀▀█ ▒█░▄▀ ▒█▀▀▀ ▒█▀▄▀█ ▒█▀▀▀█ ▒█▄░▒█\n▒█░░░ ▒█▀▀█ ▒█░░▒█ ▒█░░▒█ ░▀▀▀▄▄ ▒█▀▀▀   ▒█▄▄▄█ ▒█░░▒█ ▒█░▒█ ▒█▄▄▀   ▒█▄▄█ ▒█░░▒█ ▒█▀▄░ ▒█▀▀▀ ▒█▒█▒█ ▒█░░▒█ ▒█▒█▒█\n▒█▄▄█ ▒█░▒█ ▒█▄▄▄█ ▒█▄▄▄█ ▒█▄▄▄█ ▒█▄▄▄   ░░▒█░░ ▒█▄▄▄█ ░▀▄▄▀ ▒█░▒█   ▒█░░░ ▒█▄▄▄█ ▒█░▒█ ▒█▄▄▄ ▒█░░▒█ ▒█▄▄▄█ ▒█░░▀█");
//	string welcomeMSG = "Welcome " + userList[userId].userName + ". Please select your partner to continue with you on your journey."; 74 + user name length
	drawRawTextAtPos({static_cast<int>((SCREEN_WIDTH - 76 - userList[userId].userName.length())/ 2), 5}, "Welcome \"" + getFGAnsiCode(243, 243, 2) + userList[userId].userName + ANSI_RESET_FOREGROUND + "\". Please select your partner to continue with you on your journey.");
	while (!entered) {
		if (updated) {
			for (int i = 0; i < pokemonList.size; i++) {
				if (pokeId == i) {
					buttonColor = {255, 255, 0};
					clearArea({static_cast<int>((SCREEN_WIDTH -  50) / 2), 15 + 20}, {SCREEN_WIDTH, 15 + 20 + 4});
					drawAtPos({static_cast<int>((SCREEN_WIDTH -  50) / 2), 15 + 20}, getFGAnsiCode(25,255,255) + pokemonList.pokemons[pokeId].desc);
				} else {
					buttonColor = {255, 255, 255};
				}
				drawRawTextAtPos({static_cast<int>(SCREEN_WIDTH / 3 * i + 10 + 20 - pokemonList.pokemons[i].name.length() / 2), 12}, getFGAnsiCode(25,255,255) + pokemonList.pokemons[i].name);
				drawThinLineHollowRectangle({SCREEN_WIDTH / 3 * i + 10, 13}, 40, 40, buttonColor);
				drawAtPos({SCREEN_WIDTH / 3 * i + 10 + 1 + 10, 14 + 5}, pokemonList.pokemons[i].art.frames[0]);
			}
			updated = false;
		}
		Sleep(50); // Sleep for 70ms;
	}
	return pokeId;
}

void runMainMenu() {
	bool ended = false;
	bool changeScreen = false;
	bool hasUpdate = true;
	int menuButtonId = (userList[userId].mode == -1) ? MAIN_MENU_BUTTON_START_GAME : MAIN_MENU_BUTTON_CONTINUE_GAME;
	thread menuControlThread(menuKeyboardController, &menuButtonId, &task, &changeScreen, &hasUpdate, &ended);
	menuControlThread.detach();

	while (!ended) {
		if (changeScreen) {
			clrScr();
			changeScreen = false;
		}
		if (task == TASK_EXIT) {
			break;
		}
		if (hasUpdate) {
			soundEngine.playSyncFromAlbum(gameSound, 2);
			hasUpdate = false;
			switch (task) {
				case TASK_MAIN_MENU:
					drawMainMenu(menuButtonId);
					break;
				case TASK_CHOOSE_GAME:
					drawChoseGameMenu(menuButtonId);
					break;
				case TASK_LEADER_BOARD:
					drawLeaderBoard(menuButtonId);
					break;
				case TASK_START_GAME:
					if (userList[userId].mode == -1) {
						makeNewGame(menuButtonId);
					}
					ended = true;
					break;
				case TASK_LOGIN_MENU:
				default:
					task = TASK_LOGIN_MENU;
					ended = true;
					break;
			}
		}
		Sleep(50);
	}
	clrScr();
}

void menuLoop() {
	runMainMenu();
	if (task == TASK_START_GAME) {
		FLAG_RUNNING = true;
		runGame();
		Sleep(1000);
		clrScr();
		task = TASK_MAIN_MENU;
	}
	if (task == TASK_MAIN_MENU) {
		menuLoop();
	}
}

void loginLoop() {
	runLoginMenu();
	if (userId == -1) {
		return;
	}
	if (userList[userId].mode != -1) {
		mode = userList[userId].mode;
		boardFromString(userList[userId].stage, 8, 10);

		BOARD_START_X = (SCREEN_WIDTH / 3 * 2) / MAZE_COL;
		BOARD_START_Y = PILAR + 1;
		SCORE_BOARD_X = (SCREEN_WIDTH / 2) + 20;
		SCORE_BOARD_Y = 20;//(BOARD_START_Y + (PILAR + 1) * MAZE_ROW * 2) - 30 / 2;
		SCORE_BOARD_Y = (BOARD_START_Y + (PILAR + 1) * MAZE_ROW) - 30 / 2;

		score = userList[userId].lastScore;
		remainPair = userList[userId].remainPair;
		suggestionTry = userList[userId].suggestionTry;
	}

	if (userList[userId].pokeId == -1) {
		userList[userId].pokeId = drawChoosePokemonMenu();
//		drawChoosePokemonMenu();
		clrScr();
	}
	saveDataToFile(string(ASSET_RELATIVE_PATH) + "gameData.txt");

	if (task == TASK_MAIN_MENU) {
		menuLoop();
		clrScr();
	}
	if (task == TASK_LOGIN_MENU) {
		if (mode != -1) {
			userList[userId].mode = mode;
			userList[userId].stage = boardToString();
			userList[userId].lastScore = score;
			userList[userId].remainPair = remainPair;
			userList[userId].suggestionTry = suggestionTry;
		}
		saveDataToFile(string(ASSET_RELATIVE_PATH) + "gameData.txt");
		loginLoop();
	}
}

int main() {
	if (project_init()) {
		drawAtPos({0, 0}, "23127255 - Nguyễn Thọ Tài\n23127255 - ______________");
		drawAtPos({(SCREEN_WIDTH - 160)/ 2, 0}, getFileContent(string(TEXT_RELATIVE_PATH) + "pikamatch_background.txt"));
		Sleep(600);
		clrScr();
		loginLoop();
	}

	project_cleanup();
	return 0;
}