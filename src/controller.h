#include <conio.h>
#include <cstdlib>

#ifndef PIKACHUGAME_CONTROLLER_H
#define PIKACHUGAME_CONTROLLER_H
#endif

#ifndef PIKACHUGAME_CONSTANT_H
#include "constant.h"
#endif

const int ESC_KEY = 0x1B;
const int ENTER_KEY = 13;
const int DELETE_KEY = 83;
const int BACKSPACE_KEY = 8;

const int ARROW_UP = 0x48;
const int ARROW_LEFT = 0x4B;
const int ARROW_RIGHT = 0x4D;
const int ARROW_DOWN = 0x50;

const int CTR_ARROW_UP = 141;
const int CTR_ARROW_LEFT = 115;
const int CTR_ARROW_RIGHT = 116;
const int CTR_ARROW_DOWN = 145;
const int CTR_ENTER_KEY = 10;

const int PRE_KEY_1 = 0;
const int PRE_KEY_2 = 224;

void loginKeyboardController(char* userName, char* userPass, bool* inputPass, bool* entered) {
	int input = 0;
	while (!*entered) {
		input = getch();
		if (input == PRE_KEY_1 || input == PRE_KEY_2) {
			input = getch();
			if (input == ARROW_UP) {
				*inputPass = false;
			} else if (input == ARROW_DOWN) {
				*inputPass = true;
			} else if (input == DELETE_KEY) { // Delete Key
				if (!*inputPass) {
					memset(userName, '\0', CHAR_USER_NAME_SIZE);
				} else {
					memset(userPass, '\0', CHAR_USER_PASS_SIZE);
				}
			}
		} else {
			if (input == BACKSPACE_KEY) {
				if (!*inputPass) {
					if (strlen(userName) == 0) continue;
					userName[strlen(userName) - 1] = '\0';
				} else {
					if (strlen(userPass) == 0) continue;
					userPass[strlen(userPass) - 1] = '\0';
				}
			} else if (input == ENTER_KEY) {
				if (!*inputPass) {
					*inputPass = true;
				} else {
					*entered = true;
//					system(("start cmd /k echo Hello " + string(userName) + " with pass: " + string(userPass)).c_str());
				}
			} else {
				if (!*inputPass) {
					if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z') || (input >= '0' && input <= '9')) {
						int len = strlen(userName);
						if (len == CHAR_USER_NAME_SIZE - 1) continue;
						userName[len] = input;
						userName[len + 1] = '\0';
					}
				} else {
					if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z') || (input >= '0' && input <= '9') || input == '!') {
						int len = strlen(userPass);
						if (len == CHAR_USER_PASS_SIZE - 1) continue;
						userPass[len] = input;
						userPass[len + 1] = '\0';
					}
				}
			}
		}
	}
}

void menuKeyboardController() {

}

void gameKeyBoardController() {
}