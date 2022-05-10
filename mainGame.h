#ifndef MAIN_GAME__H_
#define MAIN_GAME__H_

#include "common.h"
#include "SDL_text.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class Box
{

public:

    int x, y;
    int preX, preY;
    int width, height;

    int speed[5] = {0, 3, 3, 3, 3};
    bool state[5] = {0, 0, 0, 0, 0};

    Box();
    ~Box();
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
    ~basicPlat();
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
    ~destinyPlat();
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
    ~goalPlat();
    goalPlat(int _x, int _y, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);
};

class deadPlat
{

public:

    Box box;
    SDL_Texture* DeadPlat;

    deadPlat();
    ~deadPlat();
    deadPlat(int _x, int _y, int length, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);
};

class Player
{

public:

    SDL_Texture* penguinNomal;
    SDL_Texture* penguinJump;
    SDL_Texture* penguinFall;

    Box box;
    int locate = 0, score = 0;
    bool fall;

    Player();
    ~Player();
    Player(int _x, int _y, int _score, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);

    void turn(direct dir, int initSpeed, Mix_Chunk *mState);
    bool handle(vector<basicPlat>& plats, destinyPlat& gplat, int& level);
    void keyboardEvent(SDL_Event e, Mix_Chunk* mJump);

    void prepare();

    bool death(vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, Mix_Chunk *mDead, Mix_Chunk *mGoal);

    bool meetPlat(basicPlat &plat);
};

void prepareNewLevel(int level, SDL_Renderer* renderer, SDL_Texture* &background);

void initGame(Player& player, vector<basicPlat>& plats, vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, destinyPlat& gplat, int level, SDL_Renderer* renderer, Mix_Chunk *mStart);

void presentScore(SDL_Renderer* renderer, TTF_Font* font, LTexture textTexture, int score);

void present(SDL_Renderer* renderer, SDL_Texture* background, Player &box, vector<basicPlat> &plats, vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, destinyPlat& gplat);

bool keyboardEvent(Player& player, Mix_Chunk* mState);

void endGame(game Game, SDL_Renderer* renderer, Mix_Chunk *mState);

void updRanking(SDL_Renderer* renderer, TTF_Font* font, LTexture textTexture, int score);

void free(vector<basicPlat> &plats, vector<goalPlat> &gPlats, vector<deadPlat> &dPlats, destinyPlat &dplat);

void releaseMemory(Player &player, vector<basicPlat> &plats, vector<goalPlat> &gPlats, vector<deadPlat> &dPlats, destinyPlat &dplat, SDL_Texture* &background,
                  Mix_Chunk* &mDead, Mix_Chunk* &mGoal, Mix_Chunk* &mJump, Mix_Chunk* &mNext, Mix_Chunk* &mStart, Mix_Chunk* &mWin);

#endif // MAIN_GAME
