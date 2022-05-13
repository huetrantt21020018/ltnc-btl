#ifndef MAIN_GAME__H_
#define MAIN_GAME__H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "common.h"
#include "plat.h"
#include "player.h"
#include "SDL_utils.h"
#include "SDL_text.h"

void prepareNewLevel(int level, SDL_Renderer* renderer, SDL_Texture* &background);

void initGame(Player& player, vector<basicPlat>& plats, vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, destinyPlat& gplat, int level, SDL_Renderer* renderer, Mix_Chunk *mStart);

// present everything

void presentScore(SDL_Renderer* renderer, TTF_Font* font, LTexture textTexture, int score);

void present(SDL_Renderer* renderer, SDL_Texture* background, Player &box, vector<basicPlat> &plats, vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, destinyPlat& gplat);

bool keyboardEvent(Player& player, Mix_Chunk* mState);

// print the status if the game is over
void endGame(game Game, SDL_Renderer* renderer, Mix_Chunk *mState);

void updRanking(SDL_Renderer* renderer, TTF_Font* font, LTexture textTexture, int score);

// memory recovery

void free(vector<basicPlat> &plats, vector<goalPlat> &gPlats, vector<deadPlat> &dPlats, destinyPlat &dplat);

void releaseMemory(Player &player, vector<basicPlat> &plats, vector<goalPlat> &gPlats, vector<deadPlat> &dPlats, destinyPlat &dplat, SDL_Texture* &background,
                  Mix_Chunk* &mDead, Mix_Chunk* &mGoal, Mix_Chunk* &mJump, Mix_Chunk* &mNext, Mix_Chunk* &mStart, Mix_Chunk* &mWin, Mix_Music* &mBeat);

#endif // MAIN_GAME
