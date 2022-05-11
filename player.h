#ifndef PLAYER__H_
#define PLAYER__H_

#include "common.h"
#include "SDL_text.h"
#include "SDL_utils.h"
#include "plat.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <vector>

class Player {

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

#endif // PLAYER
