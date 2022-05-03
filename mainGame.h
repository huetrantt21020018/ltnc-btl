#ifndef MAIN_GAME__H_
#define MAIN_GAME__H_

#include "common.h"
#include <SDL.h>
#include <vector>
#include <string>
#include <fstream>

int rnd(int a, int b);


class Box
{

public:

    int x, y;
    int preX, preY;
    int width, height;

    int speed[5] = {0, 3, 3, 3, 3};
    bool state[5] = {0, 0, 0, 0, 0};

    Box();
    Box(int _x, int _y, int _w, int _h);

    void render(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    int move();
    Box premove();

    bool fallOut();
    bool insideScreen();

    bool overlap(Box o);
    direct horizontal(Box o);
    direct vertical(Box o);
};

class basicPlat
{

public:

    Box box;
    direct dir;

    basicPlat();
    basicPlat(int _x, int _y, int length, direct dir);

    void move();

    void render(SDL_Renderer* renderer);
};

class deadPlat : public basicPlat
{

};

class Player
{

public:

    Box box;
    int locate = 0;

    Player();
    Player(int _x, int _y);

    void render(SDL_Renderer* renderer);

    void turn(direct dir, int initSpeed);
    void handle(vector<basicPlat>& plats);
    void keyboardEvent(SDL_Event e);

    bool death();

    bool meetPlat(basicPlat &plat);
};

void initGame(vector<basicPlat>& plats, int level);

void present(SDL_Renderer* renderer, Player &box, vector<basicPlat> &plats);

bool keyboardEvent(Player& player);

void handleGame(Player &player, vector<basicPlat> &plats);


#endif // MAIN_GAME

