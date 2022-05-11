#include "plat.h"

using namespace std;

// box

Box::Box() {}
Box::~Box() {}
Box::Box(int _x, int _y, int _w, int _h):
    x(_x), y(_y), width(_w), height(_h) {}

void Box::render(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
   // cout << x << ' ' << y << endl;
    SDL_Rect filled_rect;
    filled_rect.x = x;
    filled_rect.y = y;
    filled_rect.w = width;
    filled_rect.h = height;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &filled_rect);
}

int Box::move() {
    preX = x;
    preY = y;
    for(int dir = UP; dir <= RIGHT; dir = dir + 1) if(state[dir]) {
        x += stepX[dir] * speed[dir];
        y += stepY[dir] * speed[dir];

        // outside
        x = max(x, 0);
        y = max(y, 0);
        x = min(x, SCREEN_WIDTH - width);
        y = min(y, SCREEN_HEIGHT - height);
    }
    if(preX == x && preY == y) return 0;
    return 1;
}

Box Box::premove() {
    Box ret(preX, preY, width, height);
    return ret;
}

bool Box::fallOut() {
    return y + height >= SCREEN_HEIGHT;
}

bool Box::insideScreen() {
    return (0 <= x && x + width <= SCREEN_WIDTH
            && 0 <= y && y + height <= SCREEN_HEIGHT);
}

bool Box::overlap(Box o) {
    return (max(x, o.x) <= min(x+width, o.x+o.width)
            && max(y, o.y) <= min(y+height, o.y+o.height));
}

direct Box::horizontal(Box o) {
    if(!(max(y, o.y) < min(y+height, o.y+o.height))) return NONE;
    if(x + width <= o.x) return LEFT;
    if(x >= o.x+o.width) return RIGHT;
    return NONE;
}
direct Box::vertical(Box o) {
    if(!(max(x, o.x) < min(x+width, o.x+o.width))) return NONE;
    if(y + height <= o.y) return UP;
    if(y >= o.y+o.height) return DOWN;
    return NONE;
}

// basicPlat

basicPlat::basicPlat() {}

basicPlat::~basicPlat() {}

basicPlat::basicPlat(int _x, int _y, int length, direct _dir):
    box(_x, _y, length, sizeBox) {box.state[_dir] = 1; dir = _dir;}

void basicPlat::render(SDL_Renderer* renderer) {
    box.render(renderer, 157, 110, 40, 15);
}

void basicPlat::move() {
    if(!box.move()) {
        box.state[dir] = false;
        dir = opposite[dir];
        box.state[dir] = true;
    }
}

// goal Plat

goalPlat::goalPlat() {}

goalPlat::~goalPlat() {
  //  SDL_DestroyTexture(glPlat);
}

goalPlat::goalPlat(int _x, int _y, SDL_Renderer* renderer) : box(_x, _y, sizeBox, sizeBox) {
    glPlat = loadTexture("picture/icon/coin.jpg", renderer);
}

void goalPlat::render(SDL_Renderer* renderer) {
//    box.render(renderer, 252, 103, 99, 56);
    SDL_Texture* plat = glPlat;

    SDL_Rect platRect;
    SDL_QueryTexture(plat, NULL, NULL, &platRect.w, &platRect.h);
    platRect.x = box.x;
    platRect.y = box.y;
    platRect.w = box.width;
    platRect.h = box.height;

    SDL_RenderCopy(renderer, plat, NULL, &platRect);
}

// destiny Plat

destinyPlat::destinyPlat() {}

destinyPlat::~destinyPlat() {
   /// SDL_DestroyTexture(desPlat);
}

destinyPlat::destinyPlat(int _x, int _y, SDL_Renderer* renderer) : box(_x, _y, sizeBox, sizeBox) {
    desPlat = loadTexture("picture/icon/tornado.jpg", renderer);
}

void destinyPlat::render(SDL_Renderer* renderer) {
//    box.render(renderer, 252, 103, 99, 56);
    SDL_Texture* plat = desPlat;

    SDL_Rect platRect;
    SDL_QueryTexture(plat, NULL, NULL, &platRect.w, &platRect.h);
    platRect.x = box.x;
    platRect.y = box.y;
    platRect.w = box.width;
    platRect.h = box.height;

    SDL_RenderCopy(renderer, plat, NULL, &platRect);
}


// dead Plat

deadPlat::deadPlat() {}

deadPlat::~deadPlat() {
    ///SDL_DestroyTexture(DeadPlat);
}

deadPlat::deadPlat(int _x, int _y, int length, SDL_Renderer* renderer) : box(_x, _y, length, sizeBox) {
    DeadPlat = loadTexture("picture/icon/deadPlat.png", renderer);
}

void deadPlat::render(SDL_Renderer* renderer) {
    // box.render(renderer, 8, 99, 255, 26);
    SDL_Texture* plat = DeadPlat;

    SDL_Rect platRect;
    SDL_QueryTexture(plat, NULL, NULL, &platRect.w, &platRect.h);
    platRect.x = box.x;
    platRect.y = box.y;
    platRect.w = box.width;
    platRect.h = box.height;

    SDL_RenderCopy(renderer, plat, NULL, &platRect);
}
