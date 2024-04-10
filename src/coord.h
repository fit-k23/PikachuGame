#ifndef PIKACHUGAME_COORD_H
#define PIKACHUGAME_COORD_H
#endif

#ifndef PIKACHUGAME_CONSTANT_H
#include "constant.h"
#endif


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
	return {x + dx, y + dy};
}