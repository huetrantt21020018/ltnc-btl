#ifndef COMMON__H_
#define COMMON__H_

#include <string>
#include "common.h"

using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 700;

const string WINDOW_TITLE = "LTNC-BTL";

const int TIME_DELAY = 30;
const int MAXSPEED = 14;
const int sizeBox = 15;
const int initialHeight = 600 - 3 * sizeBox / 2;

enum game {START, LOSE, WIN};
enum direct {NONE, UP, DOWN, LEFT, RIGHT, DEAD};
const direct opposite[6] = {NONE, DOWN, UP, RIGHT, LEFT, DEAD};
const int stepX[6] = {0, 0, 0, -1, 1, 0};
const int stepY[6] = {0, -1, 1, 0, 0, 0};

#endif // COMMON
