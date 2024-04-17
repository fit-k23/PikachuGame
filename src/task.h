#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef PIKACHUGAME_CONTROLLER_H
#define PIKACHUGAME_CONTROLLER_H
#include <conio.h>
#include <cstdlib>

#ifndef PIKACHUGAME_CONSTANT_H
#include "constant.h"
#endif

#endif

static auto currentButton = 0;

const int LOGIN_MENU_INPUT_USERNAME = 0;
const int LOGIN_MENU_INPUT_USERPASS = 1;
const int LOGIN_MENU_INPUT_LOGIN = 2;
const int LOGIN_MENU_INPUT_SIGNIN = 3;

const int MAIN_MENU_BUTTON_CONTINUE_GAME = 0;
const int MAIN_MENU_BUTTON_START_GAME = 1;
const int MAIN_MENU_BUTTON_LEADER_BOARD = 2;
const int MAIN_MENU_BUTTON_SETTING = 3;
const int MAIN_MENU_BUTTON_LOG_OUT = 4;

const int CHOOSE_GAME_BUTTON_NORMAL = 0;
const int CHOOSE_GAME_BUTTON_COLLAPSE = 1;
const int CHOOSE_GAME_BUTTON_RETURN = 2;

const int LEADERBOARD_BUTTON_NORMAL = 0;
const int LEADERBOARD_BUTTON_COLLAPSE = 1;
const int LEADERBOARD_BUTTON_RETURN = 2;

const int TASK_LOGIN_MENU = 0;
const int TASK_CREATE_ACCOUNT = 1;
const int TASK_MAIN_MENU = 2;
const int TASK_CHOOSE_GAME = 3;
const int TASK_START_GAME = 4;
const int TASK_LEADER_BOARD = 5;
const int TASK_CONTINUE_GAME = 6;
const int TASK_SETTING_MENU = 99;
const int TASK_EXIT = 100;

static auto task = TASK_LEADER_BOARD;

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

#ifndef PIKACHUGAME_DATA_H
#include "data.h"
#endif

void loginKeyboardController(char* userName, char* userPass, int* loginButton, bool* entered, int* loginState) {
	int input;
	while (!*entered) {
		input = getch();
		if (input == PRE_KEY_1 || input == PRE_KEY_2) {
			input = getch();
			switch (input) {
				case ARROW_UP:
				case ARROW_LEFT:
					if (*loginButton > LOGIN_MENU_INPUT_USERNAME) {
						(*loginButton)--;
					}
					break;
				case ARROW_DOWN:
				case ARROW_RIGHT:
					if (*loginButton < LOGIN_MENU_INPUT_SIGNIN) {
						(*loginButton)++;
					}
					break;
				case DELETE_KEY:
					if (*loginButton == LOGIN_MENU_INPUT_USERNAME) {
						memset(userName, '\0', CHAR_USER_NAME_SIZE);
					} else if (*loginButton == LOGIN_MENU_INPUT_USERPASS) {
						memset(userPass, '\0', CHAR_USER_NAME_SIZE);
					}
					break;
				default:
					break;
			}
		} else {
			if (input == BACKSPACE_KEY) {
				if (*loginButton == LOGIN_MENU_INPUT_USERNAME) {
					if (strlen(userName) == 0) continue;
					userName[strlen(userName) - 1] = '\0';
				} else if (*loginButton == LOGIN_MENU_INPUT_USERPASS){
					if (strlen(userPass) == 0) continue;
					userPass[strlen(userPass) - 1] = '\0';
				}
			} else if (input == ENTER_KEY) {
				switch (*loginButton) {
					case LOGIN_MENU_INPUT_USERNAME:
						*loginButton = LOGIN_MENU_INPUT_USERPASS;
						break;
					case LOGIN_MENU_INPUT_USERPASS:
					case LOGIN_MENU_INPUT_LOGIN:
						*loginState = tryLogIn(string(userName), string(userPass));
						if (*loginState == 1) {
							*entered = true;
						}
						break;
					case LOGIN_MENU_INPUT_SIGNIN:
						*loginState = tryAddUser(string(userName), string(userPass));
						if (*loginState != -1) {
							*entered = true;
						}
				}
			} else {
				*loginState = -2;
				if (*loginButton == LOGIN_MENU_INPUT_USERNAME) {
					if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z') || (input >= '0' && input <= '9')) {
						size_t len = strlen(userName);
						if (len == CHAR_USER_NAME_SIZE - 1) continue;
						userName[len] = (char) input;
						userName[len + 1] = '\0';
					}
				} else if (*loginButton == LOGIN_MENU_INPUT_USERPASS){
					if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z') || (input >= '0' && input <= '9') || input == '!' || input == '#' || input == '?' || input == '@') {
						size_t len = strlen(userPass);
						if (len == CHAR_USER_PASS_SIZE - 1) continue;
						userPass[len] = (char) input;
						userPass[len + 1] = '\0';
					}
				}
			}
		}
	}
}

void reinitButtonBorder(int taskId, int &buttonBorder1, int &buttonBorder2) {
	switch (taskId) {
		case TASK_LOGIN_MENU:
			buttonBorder1 = LOGIN_MENU_INPUT_USERNAME;
			buttonBorder2 = LOGIN_MENU_INPUT_SIGNIN;
			break;
		case TASK_MAIN_MENU:
			buttonBorder1 = MAIN_MENU_BUTTON_CONTINUE_GAME;
			buttonBorder2 = MAIN_MENU_BUTTON_LOG_OUT;
			break;
		case TASK_CHOOSE_GAME:
			buttonBorder1 = CHOOSE_GAME_BUTTON_NORMAL;
			buttonBorder2 = CHOOSE_GAME_BUTTON_RETURN;
			break;
		case TASK_LEADER_BOARD:
			buttonBorder1 = LEADERBOARD_BUTTON_NORMAL;
			buttonBorder2 = LEADERBOARD_BUTTON_RETURN;
			break;
	}
}

void menuKeyboardController(int* currentButtonPtr, int* taskId, bool* changeScreen, bool *hasUpdate) {
	int input = 0;

	int buttonBorder1 = 0;
	int buttonBorder2;
	reinitButtonBorder(*taskId, buttonBorder1, buttonBorder2);

	while (input != ESC_KEY) {
		input = getch();
		if (input == PRE_KEY_1 || input == PRE_KEY_2) {
			input = getch();
			if (input == ARROW_UP) {
				if (*currentButtonPtr > buttonBorder1) {
					(*currentButtonPtr)--;
					*hasUpdate = true;
				}
			} else if (input == ARROW_DOWN) {
				if (*currentButtonPtr < buttonBorder2) {
					(*currentButtonPtr)++;
					*hasUpdate = true;
				}
			}
		} else {
			if (input == ENTER_KEY) {
				switch (*taskId) {
					case TASK_MAIN_MENU: //Task main menu
						if (*currentButtonPtr == MAIN_MENU_BUTTON_LEADER_BOARD) { //Leaderboard
							*hasUpdate = true;
							*changeScreen = true;
							*taskId = TASK_LEADER_BOARD; //Set task to leaderboard
							reinitButtonBorder(*taskId, buttonBorder1, buttonBorder2);
						} else if (*currentButtonPtr == MAIN_MENU_BUTTON_START_GAME) {
							*changeScreen = true;
							*hasUpdate = true;
							*currentButtonPtr = CHOOSE_GAME_BUTTON_RETURN;
							*taskId = TASK_CHOOSE_GAME;
							reinitButtonBorder(*taskId, buttonBorder1, buttonBorder2);
						} else if (*currentButtonPtr == MAIN_MENU_BUTTON_LOG_OUT) {
							*hasUpdate = true;
							*changeScreen = true;
							*taskId = TASK_LOGIN_MENU;
							reinitButtonBorder(*taskId, buttonBorder1, buttonBorder2);
						}
						break;
					case TASK_CHOOSE_GAME: //Task start game
						if (*currentButtonPtr == 2) {
							*taskId = TASK_MAIN_MENU;
							reinitButtonBorder(*taskId, buttonBorder1, buttonBorder2);
							*hasUpdate = true;
							*currentButtonPtr = MAIN_MENU_BUTTON_START_GAME;
						}
						break;
					case TASK_LEADER_BOARD:
						if (*currentButtonPtr == LEADERBOARD_BUTTON_RETURN) {
							*changeScreen = true;
							*taskId = TASK_MAIN_MENU;
							reinitButtonBorder(*taskId, buttonBorder1, buttonBorder2);
							*hasUpdate = true;
							*currentButtonPtr = MAIN_MENU_BUTTON_LEADER_BOARD;
						}
						break;
				}
			}
		}
	}
}