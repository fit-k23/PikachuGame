string str_repeat(string s, int n) {
	string s1 = s;
	for (int i=1; i<n;i++) s += s1;
	return s;
}

string getFileContent(const string &fileName) {
	ifstream file(fileName);
	if (!file.is_open()) return "";
	ostringstream sstr;
	sstr << file.rdbuf();
	file.close();
	return sstr.str();
}