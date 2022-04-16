#ifndef COMMON__H_
#define COMMON__H_

#include <string>
#include "common.h"

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 700;
const string WINDOW_TITLE = "LTNC-BTL";

const int TIME_DELAY = 30;
const int MAXSPEED = 14;
const int sizeBox = 15;
const int initialHeight = 600;

enum direct {NONE, UP, DOWN, LEFT, RIGHT};
const int stepX[5] = {0, 0, 0, -1, 1};
const int stepY[5] = {0, -1, 1, 0, 0};

#endif // COMMON
