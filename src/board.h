#ifndef PIKACHUGAME_BOARD_H
#define PIKACHUGAME_BOARD_H
#include <algorithm>
#include <random>
#endif

#ifndef PIKACHUGAME_COORD_H
#include "coord.h"
#endif

#ifndef PIKACHUGAME_UTILS_H
#include "utils.h"
#endif

int ROW;
int COL;
const int PADDING = 1;
int MAZE_ROW;
int MAZE_COL;

struct Box {
	char alphabet = ' ';
	bool invisible = false;
	int color = 0;
};

Coord cursor = {PADDING, PADDING};

Box** boxes;
bool** maze;

static int remainPair;

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

#ifndef PIKACHUGAME_UTILS_H
#include "utils.h"
#endif

void generateShuffledBoard() {
	int cnt[7] = {};
	int total = ROW * COL;
	int len = total;
	while (total > 0) for (int i = 0; i < 7; i++) {
		if (total <= 0) break;
		int num = 2 * getRandomCharInRange(0, 3);
		if (total >= num) {
			cnt[i] += num;
			total -= num;
		}
	}

	int count = 0;
	while (count < len) {
		for (int i = 0; i < 7; ++i) {
			if (getRandomIntInRange(0, 2) == 2) {
				i = getRandomIntInRange(0, 6);
				continue;
			}
			if (cnt[i] > 0) {
				if (count == len) {
					break;
				}
				boxes[count / COL][count % COL].alphabet = (char)(i + 'A');
				cnt[i]--;
				count++;
			}
		}
	}
}

void generateColorBoard() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			boxes[i][j].color = getRandomIntInRange(0, 11);
		}
	}
}

void setBoardSize(int row, int col) {
	uninitBoard();
	ROW = row;
	COL = col;
	MAZE_ROW = ROW + 2 * PADDING;
	MAZE_COL = COL + 2 * PADDING;
	initBoard();
	generateShuffledBoard();
	generateColorBoard();
	remainPair = ROW * COL / 2;
}
