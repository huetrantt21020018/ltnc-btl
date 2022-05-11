#ifndef PLAT__H_
#define PLAT__H_

#include "common.h"
#include "SDL_text.h"
#include "SDL_utils.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


class Box {

public:

    int x, y;
    int preX, preY;
    int width, height;

    int speed[6] = {0, 3, 3, 3, 3, 0};
    bool state[6] = {0, 0, 0, 0, 0, 0};

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

class basicPlat {

public:

    Box box;
    direct dir;

    basicPlat();
    ~basicPlat();
    basicPlat(int _x, int _y, int length, direct dir);

    void move();

    void render(SDL_Renderer* renderer);
};

class destinyPlat {
public:

    Box box;
    SDL_Texture* desPlat;

    destinyPlat();
    ~destinyPlat();
    destinyPlat(int _x, int _y, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);
};

class goalPlat {
public:

    Box box;
    SDL_Texture* glPlat;
    bool exist = true;

    goalPlat();
    ~goalPlat();
    goalPlat(int _x, int _y, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);
};

class deadPlat {

public:

    Box box;
    SDL_Texture* DeadPlat;

    deadPlat();
    ~deadPlat();
    deadPlat(int _x, int _y, int length, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);
};

#endif // PLAT
