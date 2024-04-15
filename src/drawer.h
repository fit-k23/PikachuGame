#ifndef PIKACHUGAME_DRAWER_H
#define PIKACHUGAME_DRAWER_H
#endif

#ifndef PIKACHUGAME_COORD_H
#include "coord.h"
#endif

const int LINE = 7;
const int PILAR = 3;

struct BoxPart{
	const char *top_left;
	const char *top_mid;
	const char *top_right;

	const char *mid_left;
	const char *mid_right;

	const char *bot_left;
	const char *bot_mid;
	const char *bot_right;
};

void drawRectangle(const BoxPart &parts, Coord coord, int width, int height, PikaRGB color = {-1, -1, -1},
				   PikaRGB bgColor = {-1, -1, -1}) {
	moveCursorToCoord(coord);
	string bgColorCode;
	string fgColorCode;
	if (color.r != -1) {
		fgColorCode = getFGAnsiCode(color);
	}
	if (bgColor.r != -1) {
		bgColorCode = getBGAnsiCode(bgColor);
	}
	string s = fgColorCode + parts.top_left;
	string space;
	for (int i = 0; i < width; i++) {
		s += parts.top_mid;
	}
	s += parts.top_right;
	cout << ANSI_RESET_BACKGROUND << s;

	for (int i = 0; i < height / 2; i++) {
		moveCursorToCoord({coord.x, coord.y + i + 1});
		cout << ANSI_RESET_BACKGROUND << fgColorCode << parts.mid_left
			 << bgColorCode + string(width, ' ') + ANSI_RESET_BACKGROUND << fgColorCode << parts.mid_right;
	}
	s = fgColorCode + parts.bot_left;
	for (int i = 0; i < width; i++)
		s += parts.bot_mid;
	s += parts.bot_right;
	moveCursorToCoord({coord.x, coord.y + height / 2 + 1});
	cout << ANSI_RESET_BACKGROUND << s;
	moveCursorToCoord({0, 0}); //Resting coord
}

void drawRoundCornerRectangle(Coord coord, int width, int height, PikaRGB color = {-1, -1, -1},
							  PikaRGB bgColor = {-1, -1, -1}, BoxPart part = {"╭", "─", "╮", "│", "│", "╰", "─", "╯"}) {
	part.top_left = "╭";
	part.top_right = "╮";
	part.bot_left = "╰";
	part.bot_right = "╯";
	drawRectangle(part, coord, width, height, color, bgColor);
}

//void drawChoppedCornerRectangle(Coord coord, int width, int height, PikaRGB bgColor = {-1,-1,-1}, BoxPart part = {"╭","─","╮","│","│","╰","─","╯"}) {
//	part.top_left = "╱";
//	part.top_right = "╲";
//	part.bot_left = "╲";
//	part.bot_right = "╱";
//	drawRectangle(part, coord, width, height, bgColor);
//} Look bad

void drawBoxyLineRectangle(Coord coord, int width, int height, PikaRGB color = {-1, -1, -1},
						   PikaRGB bgColor = {-1, -1, -1}) {
	drawRectangle({"▄", "▄", "▄", "█", "█", "▀", "▀", "▀"}, coord, width, height, color, bgColor);
}

void drawHeavyLineRectangle(Coord coord, int width, int height, PikaRGB color = {-1, -1, -1},
							PikaRGB bgColor = {-1, -1, -1}) {
	drawRectangle({"┏", "━", "┓", "┃", "┃", "┗", "━", "┛"}, coord, width, height, color, bgColor);
}

void drawHeavyLineRoundCornerRectangle(Coord coord, int width, int height, PikaRGB color = {-1, -1, -1},
									   PikaRGB bgColor = {-1, -1, -1}) {
	drawRoundCornerRectangle(coord, width, height, color, bgColor, {"┏", "━", "┓", "┃", "┃", "┗", "━", "┛"});
}

void drawDoubleLineRectangle(Coord coord, int width, int height, PikaRGB color = {-1, -1, -1},
							 PikaRGB bgColor = {-1, -1, -1}) {
	drawRectangle({"╔", "═", "╗", "║", "║", "╚", "═", "╝"}, coord, width, height, color, bgColor);
}

void drawDoubleLineHollowRectangle(Coord coord, int width, int height, PikaRGB color = {-1, -1, -1}) {
	moveCursorToCoord(coord);
	string fgColorCode;
	if (color.r != -1) {
		fgColorCode = getFGAnsiCode(color);
	}
	string s = fgColorCode + "╔";
	string space;
	for (int i = 0; i < width; i++) {
		s += "═";
	}
	s += "╗";
	cout << ANSI_RESET << s;

	for (int i = 0; i < height / 2; i++) {
		moveCursorToCoord({coord.x, coord.y + i + 1});
		cout << fgColorCode << "║" << ANSI_RESET;
		moveCursorToCoord({coord.x + width + 1, coord.y + i + 1});
		cout << fgColorCode << "║" << ANSI_RESET;
	}
	s = fgColorCode + "╚";
	for (int i = 0; i < width; i++)
		s += "═";
	s += "╝";
	cout << s;
	moveCursorToCoord({coord.x, coord.y + height / 2 + 1});
	moveCursorToCoord({0, 0}); //Resting coord
}

void drawThinLineRectangle(Coord coord, int width, int height, PikaRGB color = {-1, -1, -1},
						   PikaRGB bgColor = {-1, -1, -1}) {
	drawRectangle({"┌", "─", "┐", "│", "│", "└", "─", "┘"}, coord, width, height, color, bgColor);
}

void drawAtPos(Coord coord, const string &s) {
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

void drawTextAtPos(Coord coord, const string &s) {
//	string foreground;
//	if (fg.r != -1) {
//		foreground = getFGAnsiCode(fg);
//	}
//	if (fg.r == -2) {
//		foreground = ANSI_RESET_FOREGROUND;
//	}
//	string background;
//	if (bg.r != -1) {
//		background = getBGAnsiCode(bg);
//	}
	moveCursorToCoord(coord);
	cout << s;
}

void drawAtPosTransparent(Coord coord, const string &s) {
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

struct AnsiArt{
	vector<string> frames;
	unsigned long long sleepTime = 10; //in ms
	bool loop = false;
	bool isDone = false;

	int getFramesAmount() const;

	int currentFrame = 0;

	void addFrame(const string &frame);

	string nextFrame();
};

int AnsiArt::getFramesAmount() const {
	return frames.size(); // NOLINT(*-narrowing-conversions)
}

void AnsiArt::addFrame(const string &frame) {
	frames.push_back(frame);
}

string AnsiArt::nextFrame() {
	if (currentFrame == getFramesAmount() - 1) {
		if (!loop) {
			isDone = true;
		} else {
			currentFrame = 0;
		}
	} else {
		currentFrame++;
	}
	return frames[currentFrame];
}

void drawAnsiArt(AnsiArt &art, Coord coord, bool transparent = false) {
	if (art.isDone) return;
	if (!transparent) drawAtPos(coord, art.nextFrame());
	else drawAtPosTransparent(coord, art.nextFrame());
//	if (art.sleepTime != 0) {
//		Sleep(art.sleepTime);
//	}
}

//Source: friendly neighbourhood!
void readAnsiFile(const string &filePath, AnsiArt &art) {
	ifstream ifs(filePath.c_str());
	if (ifs.is_open()) {
		int width = 0, height = 0;
		ifs >> width;
		ifs >> height;
//		cout << width << " " << height << "\n";
		ifs.ignore();
		int nFrame = width * height / (8 * 4);
		string frame;
		while (nFrame-- > 0 && !ifs.eof()) {
			string line;
			getline(ifs, line);
			while (line != "@ANSI" && !line.empty() && !ifs.eof()) {
				line += '\n';
				frame += line;
				getline(ifs, line);
			}

			if (!ifs.eof()) {
				art.addFrame(frame);
			}
			frame = "";
		}

//		cout << art.getFramesAmount() << "\n";
		ifs.close();
	}

//	cout << art.frames[0] << "\n";
	// while (!art.isDone) {
	//     cout << art.nextFrame() << "\n";
	// }
}