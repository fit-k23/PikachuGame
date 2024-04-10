#ifndef PIKACHUGAME_DRAWER_H
#define PIKACHUGAME_DRAWER_H
#endif

#ifndef PIKACHUGAME_COORD_H
#include "coord.h"
#endif

const int LINE = 7;
const int PILAR = 3;

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
	moveCursorToCoord(coord);
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
		moveCursorToCoord({coord.x, coord.y + i + 1});
		cout << ANSI_RESET_BACKGROUND << parts.mid_left << bgColorCode + string(width, ' ') + ANSI_RESET_BACKGROUND + parts.mid_right;
	}
	s = parts.bot_left;
	for (int i = 0; i < width; i++)
		s += parts.bot_mid;
	moveCursorToCoord({coord.x, coord.y + height / 2 + 1});
	s += parts.bot_right;
	cout << ANSI_RESET_BACKGROUND << s;
	moveCursorToCoord({0,0}); //Resting coord
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
	moveCursorToCoord(coord);
	string s = "╔";
	string space;
	for (int i = 0; i < width; i++) {
		s += "═";
	}
	cout << s << "╗";

	for (int i = 0; i < height / 2; i++) {
		moveCursorToCoord({coord.x, coord.y + i + 1});
		cout << "║";
		moveCursorToCoord({coord.x + width + 1, coord.y + i + 1});
		cout << "║";
	}
	s = "╚";
	for (int i = 0; i < width; i++)
		s += "═";
	moveCursorToCoord({coord.x, coord.y + height / 2 + 1});
	s += "╝";
	cout << s;
	moveCursorToCoord({0,0}); //Resting coord
}

void drawThinLineRectangle(Coord coord, int width, int height, PikaRGB bgColor = {-1,-1,-1}) {
	drawRectangle({"┌", "─", "┐", "│", "│", "└", "─", "┘"}, coord, width, height, bgColor);
}

void drawAtPos(Coord coord, const string& s) {
	istringstream sstr(s);
	string temp;
	while (!sstr.eof()) {
		getline(sstr, temp, '\n');
		moveCursorToCoord(coord);
		cout << ANSI_RESET_BACKGROUND << temp;
		coord.y++;
	}
	moveCursorToCoord({0, 0});
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

		moveCursorToCoord({coord.x + dx, coord.y});
		cout << temp << ANSI_RESET_BACKGROUND << ANSI_RESET_FOREGROUND;
		coord.y++;
	}
}