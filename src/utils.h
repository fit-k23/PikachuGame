#ifndef PIKACHUGAME_UTILS_H
#define PIKACHUGAME_UTILS_H

#include <filesystem>
#include <string>
#include <fstream>

using namespace std;
#endif

// Repeat a string multi times, Eg: str_repeat("a",3) -> "aaa"
// Name suggested by PHP >:)! PHP RULE!11!
string str_repeat(string s, int n) {
	string s1 = s;
	for (int i = 1; i < n; i++) s += s1;
	return s;
}

// Get File's Content, name suggested by PHP :D! PHP RULE!!!!
string getFileContent(const string &fileName) {
	ifstream file(fileName);
	if (!file.is_open()) return "";
	ostringstream sstr;
	sstr << file.rdbuf();
	file.close();
	return sstr.str();
}

#include <random>

int getRandomIntInRange(int range_a, int range_b) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(range_a, range_b + 1);
	return (int) round(dist(mt));
	//return range_a + rand() % (range_b - range_a + 1);
}

char getRandomCharInRange(int range_a = 65, int range_b = 90) {
	return (char) getRandomIntInRange(range_a, range_b);
}

struct PikaRGB{
	int r;
	int g;
	int b;
};

PikaRGB getSuitAbleFGColor(PikaRGB in) {
	PikaRGB out = {};
	if (in.r < 255 / 2) {
		out.r = 255;
	} else out.r = 0;
	if (in.g < 255 / 2) {
		out.g = 255;
	} else out.r = 0;
	if (in.b < 255 / 2) {
		out.b = 255;
	} else out.r = 0;
	return out;
}

string getBGAnsiCode(int r, int g, int b) {
	return "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
}

string getBGAnsiCode(PikaRGB rgb) {
	return getBGAnsiCode(rgb.r, rgb.g, rgb.b);
}

string getFGAnsiCode(int r, int g, int b) {
	return "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
}

string getFGAnsiCode(PikaRGB rgb) {
	return getFGAnsiCode(rgb.r, rgb.g, rgb.b);
}

bool dirExist(const char *dirPath) {
	return filesystem::is_directory(dirPath);
}

// https://stackoverflow.com/a/24750132/24078702
bool fileExist(const char *filePath) {
	return filesystem::exists(filePath);
}