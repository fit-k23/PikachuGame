#ifndef PIKACHUGAME_BOARD_H
#define PIKACHUGAME_BOARD_H
#endif

#ifndef PIKACHUGAME_COORD_H
#include "coord.h"
#endif

int ROW;
int COL;
const int PADDING = 1;
int MAZE_ROW;
int MAZE_COL;

struct Box {
	char alphabet = ' ';
	bool invisible = false;
};

Coord cursor = {PADDING, PADDING};

Box** boxes;
bool** maze;

void initBoard() {
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
}

void uninitBoard() {
	if (boxes != nullptr) {
		for (int i = 0; i < ROW; i++) delete[] boxes[i];
		delete boxes;
	}
	if (maze != nullptr) {
		for (int i = 0; i < MAZE_ROW; i++) delete[] maze[i];
		delete maze;
	}
}

void setBoardSize(int row, int col) {
	uninitBoard();
	ROW = row;
	COL = col;
	MAZE_ROW = ROW + 2 * PADDING;
	MAZE_COL = COL + 2 * PADDING;
	initBoard();
}

#ifndef PIKACHUGAME_UTILS_H
#include "utils.h"
#endif

void pushRandomCharsToBoard() { //Todo: make the part actually work!
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			boxes[i][j].alphabet = getRandomCharInRange(65, 70);
		}
	}
}