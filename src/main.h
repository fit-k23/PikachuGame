#ifndef PIKACHUGAME_MAIN_H
#define PIKACHUGAME_MAIN_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

#ifndef PIKACHUGAME_AUDIO_H
#include "audio.h"
#endif

#ifndef PIKACHUGAME_BFS_H
#include "bfs.h"
#endif

#ifndef PIKACHUGAME_CONSOLE_H
#include "console.h"
#endif

#ifndef PIKACHUGAME_CONSTANT_H
#include "constant.h"
#endif

#ifndef PIKACHUGAME_COORD_H
#include "coord.h"
#endif

#ifndef PIKACHUGAME_DATA_H
#include "data.h"
#endif

#ifndef PIKACHUGAME_DRAWER_H
#include "drawer.h"
#endif

#ifndef PIKACHUGAME_TASK_H
#include "task.h"
#endif

#ifndef PIKACHUGAME_UTILS_H
#include "utils.h"
#endif

#endif

bool project_init();
void project_cleanup();

