#ifndef MAIN_GAME__H_
#define MAIN_GAME__H_

#include "common.h"
#include <SDL.h>
#include <SDL_image.h>
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

class destinyPlat
{
public:

    Box box;
    SDL_Texture* desPlat;

    destinyPlat();
    destinyPlat(int _x, int _y, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);
};

class goalPlat
{
public:

    Box box;
    SDL_Texture* glPlat;
    bool exist = true;

    goalPlat();
    goalPlat(int _x, int _y, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);
};

class deadPlat
{

public:

    Box box;
    SDL_Texture* DeadPlat;

    deadPlat();
    deadPlat(int _x, int _y, int length, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);
};

class Player
{

public:

    SDL_Texture* penguinNomal;
    SDL_Texture* penguinJump;
    SDL_Texture* penguinFall;
    SDL_Texture* penguinLeft;
    SDL_Texture* penguinRight;

    Box box;
    int locate = 0, score = 0;
    bool fall;

    Player();
    Player(int _x, int _y, int _score, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);

    void turn(direct dir, int initSpeed);
    bool handle(vector<basicPlat>& plats, destinyPlat& gplat, int& level);
    void keyboardEvent(SDL_Event e);

    void prepare();

    bool death(vector<deadPlat>& dPlats, vector<goalPlat>& gPlats);

    bool meetPlat(basicPlat &plat);
};

void prepareNewLevel(int level, SDL_Renderer* renderer, SDL_Texture* &background);

void initGame(Player& player, vector<basicPlat>& plats, vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, destinyPlat& gplat, int level, SDL_Renderer* renderer);

void present(SDL_Renderer* renderer, SDL_Texture* background, Player &box, vector<basicPlat> &plats, vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, destinyPlat& gplat);

bool keyboardEvent(Player& player);

void endGame(game Game, SDL_Renderer* renderer);

#endif // MAIN_GAME
