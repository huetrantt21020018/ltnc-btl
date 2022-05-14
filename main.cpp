#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#include <iostream>
#include <vector>

#include "common.h"
#include "SDL_utils.h"
#include "SDL_text.h"
#include "mainGame.h"

using namespace std;

int main(int argc, char* argv[]) {

    // init SDL

    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    TTF_Font* font = TTF_OpenFont("fontText/Aller_lt.ttf", sizeBox);
    LTexture textTexture;

    //The sound effects that will be used
    Mix_Music *mBeat = Mix_LoadMUS("sound/bgm.wav");

    Mix_Chunk *mDead = Mix_LoadWAV("sound/dead.wav");
    Mix_Chunk *mGoal = Mix_LoadWAV("sound/goal.wav");
    Mix_Chunk *mJump = Mix_LoadWAV("sound/jump.wav");
    Mix_Chunk *mStart = Mix_LoadWAV("sound/start.wav");
    Mix_Chunk *mWin = Mix_LoadWAV("sound/win.wav");
    Mix_Chunk *mNext = Mix_LoadWAV("sound/nextLevel.wav");

    // init game

    Mix_PlayMusic( mBeat, -1 );

    Player player((SCREEN_WIDTH + 5*sizeBox)/2, initialHeight, 0, renderer);
    vector<basicPlat> plats;
    vector<deadPlat> dPlats;
    vector<goalPlat> gPlats;
    destinyPlat dplat;

    while(true) {

        int level = 1;
        int speaker = 1;

        initGame(player, plats, dPlats, gPlats, dplat, level, renderer, mStart);

        SDL_Texture* background = loadTexture("picture/background/background1.png", renderer);

        if(makeMenu(renderer, font, background)) break;

        present(renderer, background, player, plats, dPlats, gPlats, dplat);
        prepareNewLevel(level, renderer, background);

        // game loop

        while(!player.death(dPlats, gPlats, mDead, mGoal)) {
            present(renderer, background, player, plats, dPlats, gPlats, dplat);
            presentScore(renderer, font, textTexture, player.score, speaker);
            player.prepare();

            if(keyboardEvent(player, mJump, speaker)) break;
            if(player.handle(plats, dplat, level) && level <= 3) {
                    // next level
                initGame(player, plats, dPlats, gPlats, dplat, level, renderer, mNext);
                present(renderer, background, player, plats, dPlats, gPlats, dplat);
                prepareNewLevel(level, renderer, background);
            }
            if(level > 3) break;

            SDL_Delay(TIME_DELAY);
        }

        // end game

        present(renderer, background, player, plats, dPlats, gPlats, dplat);
        presentScore(renderer, font, textTexture, player.score, speaker);
        SDL_Delay(1000);

        if(player.death(dPlats, gPlats, NULL, NULL)) endGame(LOSE, renderer, NULL, speaker);
        else {
            endGame(WIN, renderer, mWin, speaker);
            updRanking(renderer, font, textTexture, player.score);
        }
        presentScore(renderer, font, textTexture, player.score, speaker);

        // release memory

        releaseMemory(player, plats, gPlats, dPlats, dplat, background, mDead, mGoal, mJump, mNext, mStart, mWin, mBeat);
    }

    quitSDL(window, renderer);
    return 0;
}
