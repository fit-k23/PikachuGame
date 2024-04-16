#ifndef PIKACHUGAME_CONSTANT_H
#define PIKACHUGAME_CONSTANT_H

static const char *ANSI_RESET_BACKGROUND = "\033[49m";
static const char *ANSI_RESET_FOREGROUND = "\033[39m";
static const char *ANSI_RESET = "\033[0m";

//static const char* NORMAL_ANSI = "\033[38;2;255;255;255m\033[49m";
static const char *NORMAL_ANSI = "";
static const char *SELECT_COLOR_ANSI = "\033[48;2;123;23;131m";
static const char *CURSOR_COLOR_ANSI = "\033[48;2;255;255;255m\x1B[38;2;0;0;0m";
//gcc -O2 -c
static const int CHAR_USER_NAME_SIZE = 21;
static const int CHAR_USER_PASS_SIZE = 21;

static const char *ASSET_RELATIVE_PATH = "asset/";
static const char *SOUND_RELATIVE_PATH = "asset/sound/";
static const char *MUSIC_RELATIVE_PATH = "asset/music/";

const string BUTTON_MODE_NORMAL = "█▄░█ █▀█ █▀█ █▀▄▀█ ▄▀█ █░░\n█░▀█ █▄█ █▀▄ █░▀░█ █▀█ █▄▄"; // Normal
const string BUTTON_MODE_COLLAPSE = "█▀▀ █▀█ █░░ █░░ ▄▀█ █▀█ █▀ █▀▀\n█▄▄ █▄█ █▄▄ █▄▄ █▀█ █▀▀ ▄█ ██▄"; // Collapse
const string BUTTON_RETURN = "█▀█ █▀▀ ▀█▀ █░█ █▀█ █▄░█\n█▀▄ ██▄ ░█░ █▄█ █▀▄ █░▀█"; // Return

const string BUTTON_START_GAME = "█▀ ▀█▀ ▄▀█ █▀█ ▀█▀   █▀▀ ▄▀█ █▀▄▀█ █▀▀\n▄█ ░█░ █▀█ █▀▄ ░█░   █▄█ █▀█ █░▀░█ ██▄"; // Start Game
const string BUTTON_CONTINUE_GAME = "█▀▀ █▀█ █▄░█ ▀█▀ █ █▄░█ █░█ █▀▀   █▀▀ ▄▀█ █▀▄▀█ █▀▀\\n█▄▄ █▄█ █░▀█ ░█░ █ █░▀█ █▄█ ██▄   █▄█ █▀█ █░▀░█ ██▄"; // Continue Game
const string BUTTON_LEADERBOARD = "█░░ █▀▀ ▄▀█ █▀▄ █▀▀ █▀█ █▄▄ █▀█ ▄▀█ █▀█ █▀▄\n█▄▄ ██▄ █▀█ █▄▀ ██▄ █▀▄ █▄█ █▄█ █▀█ █▀▄ █▄▀"; // Leaderboard
const string BUTTON_SETTING = "█▀ █▀▀ ▀█▀ ▀█▀ █ █▄░█ █▀▀\\n▄█ ██▄ ░█░ ░█░ █ █░▀█ █▄█"; // Setting
const string BUTTON_SAVE_AND_LOGOUT = "█▀ ▄▀█ █░█ █▀▀   ▄▀█ █▄░█ █▀▄   █░░ █▀█ █▀▀ █▀█ █░█ ▀█▀\n▄█ █▀█ ▀▄▀ ██▄   █▀█ █░▀█ █▄▀   █▄▄ █▄█ █▄█ █▄█ █▄█ ░█░"; // Save and loggout

#endif
