// input.cpp : This file contains the 'main' function. Program execution begins and ends there.
#ifndef PIKACHUGAME_DATA_H
#define PIKACHUGAME_DATA_H

#include <iostream>
#include<fstream>
#include <string>
#include <cstring>
#include <sstream>

#endif

#ifndef PIKACHUGAME_CONSTANT_H
#include "constant.h"
#endif

using namespace std;

//struct User be datatype of each account,
struct User {
	string stage;
	string userName;
	string userPass;
	int pokeId = -1;
	int score[2]{0,0};
	int mode = -1; // -1 no save, 0 normal, 1 collapse
	int lastScore = 0;
	int remainPair = 0;
	int suggestionTry = 3;
};

static User* userList;

static int userId = -1;

//Struct test be datatype of a value which will check the input:
//Login Succcesfully(nameCheck=true,passCheck=true), Wrong Password(nameCheck=true,passCheck=false), User doesn't exist(nameCheck=false)
struct LoginCheck {
	bool nameCheck;
	bool passCheck;
};

struct PikaRank {
	int userId = -1;
	PikaRank *next = nullptr;
};

const int DEFAULT_MAX_PLAYER = 20;
bool DEFAULT_ENABLE_SOUND = true;
bool DEFAULT_ENABLE_MUSIC = true;

const int LEVEL_MAX = 2;

static struct Config{
	bool enableSound = true;
	bool enableMusic = true;
	int nUser = 0;
	int maxUser = DEFAULT_MAX_PLAYER;
} config;

static bool deleteUserListPtr() {
	delete[] userList;
	return true;
}

int searchUser(const string& userName) {
	for (int i = 0; i < config.nUser; i++) {
		if (userName == userList[i].userName) return i;
	}
	return -1;
}

int tryAddUser(const string& userName, const string& userPass, int pokeId = -1, int score1 = 0, int score2 = 0, int mode = -1) {
	if (searchUser(userName) != -1) return -1;
	userList[config.nUser++] = {"", userName, userPass, pokeId, {score1, score2}, mode};
	return config.nUser - 1;
}

// -1: account not exist
// 0: wrong password
// 1: succeed login
int tryLogIn(const string& userName, const string& userPass) {
	int uid = searchUser(userName);
	if (uid == -1) {
		return -1;
	}
	return userList[uid].userPass == userPass;
}

// -1 account not found
// other: user id
bool login(const string& userName, const string& userPass) {
	int uid = searchUser(userName);
	if (uid == -1) {
		return -1;
	}
	return uid;
}

//this function will read all account in the data file,and it will be used for "EasyRank","NormalRank","HardRank" to find top highest score
bool loadDataFromFile(const string &fileName) {
	ifstream file(fileName.c_str());
	if (!file.is_open()) {
		return false;
	}
	string temp;
	getline(file, temp, ';');
	int maxUser = 0;
	if (!temp.empty()) {
		maxUser = stoi(temp);
	}
	if (maxUser < DEFAULT_MAX_PLAYER) {
		maxUser = DEFAULT_MAX_PLAYER;
	}

	if (userList != nullptr) {
		deleteUserListPtr();
	}
	userList = new User[maxUser];
	config.maxUser = maxUser;

	getline(file, temp, ';');
	config.enableSound = temp.empty() ? DEFAULT_ENABLE_SOUND : (bool) stoi(temp);
	getline(file, temp, '\n');
	config.enableMusic = temp.empty() ? DEFAULT_ENABLE_MUSIC : (bool) stoi(temp);


	while (!file.eof()) {
		config.nUser++;
		User* user = &userList[config.nUser - 1];
		getline(file, user->userName, ';');
		if (user->userName.empty()) {
			config.nUser--;
			getline(file, temp, '\n'); //Get rid of broken data
			continue;
		}
		getline(file, user->userPass, ';');
		file >> user->pokeId;
		file.ignore();
		stringstream ss;
		getline(file, temp, ';');
		ss << temp + ",";
		int l = 0;
		while (l < LEVEL_MAX) {
			getline(ss, temp, ',');
			user->score[l++] = stoi(temp);
		}
		file >> user->mode;
		if (user->mode == -1) {
			getline(file, temp, '\n'); //Get rid of useless info
			continue;
		}
		file.ignore();
		file >> user->lastScore;
		file.ignore();
		file >> user->remainPair;
		file.ignore();
		file >> user->suggestionTry;
		file.ignore();
		getline(file, user->stage, '\n');
	}
	file.close();
	return true;
}

static bool saveDataToFile(const string &fileName) {
	ofstream file(fileName.c_str(), ofstream::trunc);
	if (!file.is_open()) {
		return false;
	}
	file << config.maxUser << ';';
	file << (config.enableSound ? '1' : '0') << ';';
	file << (config.enableMusic ? '1' : '0') << '\n';

	for (int i = 0; i < config.nUser; i++) {
		if (userList[i].userName.empty()) {
			continue;
		}
		file << userList[i].userName << ";";
		file << userList[i].userPass << ";";
		file << userList[i].pokeId << ";";
		for (int l = 0; l < LEVEL_MAX - 1; l++) {
			file << userList[i].score[l] << ',';
		}
		file << userList[i].score[LEVEL_MAX - 1] << ';';
		file << userList[i].mode << ";";
		if (userList[i].mode == -1) {
			file << '\n';
			continue;
		}
		file << userList[i].lastScore << ";";
		file << to_string(userList[i].remainPair) << ";";
		file << to_string(userList[i].suggestionTry) << ";";
		file << userList[i].stage << '\n';
	}

	file.close();
	return true;
}

void bobbleInsert(int arr[10], int n, int k, int score = 0) {
	for (int i = 0; i < n; i++) {
		if (arr[i] == -1) {
			arr[i] = k;
			break;
		}
		if (userList[arr[i]].score[score] < userList[k].score[score]) {
			int temp = arr[i];
			arr[i] = k;
			k = temp;
		}
	}
}

//void recursiveBobbleInsert(int arr[], int n, int k, int mode = 0) {
//	if (n < 0) return;
//	if (arr[n - 1] == -1) {
//		arr[n - 1] = k;
//		return;
//	}
//	if (userList[arr[n - 1]].score[mode] < userList[k].score[mode]) {
//		int temp = arr[n - 1];
//		arr[n - 1] = k;
//		k = temp;
//	}
//	return recursiveBobbleInsert(arr, n - 1, k, mode);
//}

// mode = 0 -> normal
// mode = 1 -> collapse
void getTopRank(int top[10], int mode = 0) {
	for (int i = 0; i < 10; i++) {
		top[i] = -1;
	}
	int s = config.nUser;
	for (int i = 0; i < s; i++) {
		bobbleInsert(top, 10, i, mode);
	}

}
