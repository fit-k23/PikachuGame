#include <cstdlib>
// Repeat a string multi times, Eg: str_repeat("a",3) -> "aaa"
// Name suggested by PHP >:)! PHP RULE!11!
string str_repeat(string s, int n) {
	string s1 = s;
	for (int i=1; i<n;i++) s += s1;
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

int getRandomIntInRange(int range_a, int range_b) {
	return range_a + rand() % (range_b - range_a + 1);
}

char getRandomCharInRange(int range_a = 65, int range_b = 90) {
	return (char) getRandomIntInRange(range_a, range_b);
}