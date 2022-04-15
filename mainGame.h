#ifndef MAIN_GAME__H_
#define MAIN_GAME__H_

#include "common.h"
#include <SDL.h>
#include <vector>

int rnd(int a, int b);


class Box
{

public:

    int x, y;
    int preX, preY;
    int width, height;

    int speed[4] = {1, 1, 1, 1};
    bool state[4] = {0, 0, 0, 0};

    Box();
    Box(int _x, int _y, int _w, int _h);

    void render(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    void move();
    Box premove();

    bool fallOut();
    bool insideScreen();

    bool overlap(Box o);
    direct horizontal(Box o);
    direct vertical(Box o);
};

class PlatBasic
{

public:

    Box box;

    PlatBasic();
    PlatBasic(int _x, int _y, int length);

    void render(SDL_Renderer* renderer);
};

class Player
{

public:

    Box box;

    Player();
    Player(int _x, int _y);

    void render(SDL_Renderer* renderer);

    void turn(direct dir, int initSpeed);
    void handle(vector<PlatBasic>& plats);
    void keyboardEvent(SDL_Event e);

    bool death();

    bool meetPlat(PlatBasic &plat);
};

void initGame(vector<PlatBasic>& plats);

void present(SDL_Renderer* renderer, Player &box, vector<PlatBasic> &plats);

bool keyboardEvent(Player& player);

void handleGame(Player &player, vector<PlatBasic> &plats);


#endif // MAIN_GAME

