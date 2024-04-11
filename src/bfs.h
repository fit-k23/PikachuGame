#ifndef PIKACHUGAME_BFS_H
#define PIKACHUGAME_BFS_H
#endif

#ifndef PIKACHUGAME_COORD_H
#include "coord.h"
#endif

#ifndef PIKACHUGAME_BOARD_H

#include "board.h"

#endif

#include <cmath>
#include <queue>

struct Selector {
	Coord c1;
	Coord c2;

	void reset();
};

void Selector::reset() {
	c1 = {-1, -1};
	c2 = {-1, -1};
}

Selector selector{{-1, -1},
				  {-1, -1}};

// Arrays to represent change in rows and columns
// DOWN, LEFT, RIGHT, UP
// 0 1 2 3
// 0 - 3 DOWN - UP -> Counter: 3 - 0 = 3
// 1 - 2 LEFT - RIGHT -> Counter: 3 - 2 = 1

const int dr[4] = {1, 0, 0, -1};
const int dc[4] = {0, -1, 1, 0};

struct Path {
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
	if (!maze[src.y][src.x] || !maze[dest.y][dest.x]) {// || isBlocked(dest) || isBlocked(src)) {
		return Path{{}, -1, -1};
	}
	std::queue<Path> q;
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