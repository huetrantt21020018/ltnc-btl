#ifndef MAIN_GAME__H_
#define MAIN_GAME__H_

#include "common.h"
#include <SDL.h>



class Box
{
    int x, y;
    int width, height;

public:

    int speed[4] = {1, 1, 1, 1};
    bool state[4] = {0, 0, 0, 0};

    Box();
    Box(int _x, int _y, int _w, int _h);

    void render(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    void move();

    bool fallOut();
    bool insideScreen();
};

class Player
{
    Box box;

public:

    Player();
    Player(int _x, int _y);

    void render(SDL_Renderer* renderer);

    void turn(direct dir, int initSpeed);
    void handle();
    void keyboardEvent(SDL_Event e);

    bool death();
};

class PlatBasic
{
    Box box;

public:

    PlatBasic();
    PlatBasic(int _x, int _y, int length);

    void render(SDL_Renderer* renderer);
};

void present(SDL_Renderer* renderer, Player &box);

bool keyboardEvent(Player& player);


#endif // MAIN_GAME

