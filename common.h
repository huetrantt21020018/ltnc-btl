#ifndef COMMON__H_
#define COMMON__H_

#include <string>
#include "common.h"

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 700;
const string WINDOW_TITLE = "LTNC-BTL";

const int TIME_DELAY = 60;
const int MAXSPEED = 15;
const int sizeBox = 15;
const int initialHeight = 600;

enum direct {UP, DOWN, LEFT, RIGHT};
const int stepX[4] = {0, 0, -1, 1};
const int stepY[4] = {-1, 1, 0, 0};

#endif // COMMON
