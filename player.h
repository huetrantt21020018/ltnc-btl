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
    int locate = 0; // the plat that the player standing above
    int score = 0;
    int loa;
    bool fall;

    Player();
    ~Player();
    Player(int _x, int _y, int _score, SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);

    // player control
    void turn(direct dir, int initSpeed, Mix_Chunk *mState);
    void keyboardEvent(SDL_Event e, Mix_Chunk* mJump, int& speaker);

    // handle the situation if you touch the plat
    bool handle(vector<basicPlat>& plats, destinyPlat& gplat, int& level);

    // change direction and speed
    void prepare();

    // check if the player broke the rules
    bool death(vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, Mix_Chunk *mDead, Mix_Chunk *mGoal);

};

#endif // PLAYER
