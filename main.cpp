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

    TTF_Font* font = TTF_OpenFont( "fontText/Aller_lt.ttf", sizeBox);
    LTexture textTexture;

    //The sound effects that will be used
    Mix_Chunk *mDead = Mix_LoadWAV( "sound/dead.wav" );
    Mix_Chunk *mGoal = Mix_LoadWAV( "sound/goal.wav" );
    Mix_Chunk *mJump = Mix_LoadWAV( "sound/jump.wav" );
    Mix_Chunk *mStart = Mix_LoadWAV( "sound/start.wav" );
    Mix_Chunk *mWin = Mix_LoadWAV( "sound/win.wav" );
    Mix_Chunk *mNext = Mix_LoadWAV( "sound/nextLevel.wav" );

    // init game

    Player player((SCREEN_WIDTH + 5*sizeBox)/2, initialHeight, 0, renderer);
    vector<basicPlat> plats;
    vector<deadPlat> dPlats;
    vector<goalPlat> gPlats;
    destinyPlat dplat;

    int level = 1;
    initGame(player, plats, dPlats, gPlats, dplat, level, renderer, mStart);

    SDL_Texture* background = loadTexture("picture/background/background1.png", renderer);

    present(renderer, background, player, plats, dPlats, gPlats, dplat);
    endGame(START, renderer, NULL);
    present(renderer, background, player, plats, dPlats, gPlats, dplat);
    prepareNewLevel(level, renderer, background);

    // game loop

    while(!player.death(dPlats, gPlats, mDead, mGoal)) {
        present(renderer, background, player, plats, dPlats, gPlats, dplat);
        presentScore(renderer, font, textTexture, player.score);
        player.prepare();
        if(keyboardEvent(player, mJump)) break;
        if(player.handle(plats, dplat, level) && level <= 3)
        {
            initGame(player, plats, dPlats, gPlats, dplat, level, renderer, mNext);
            present(renderer, background, player, plats, dPlats, gPlats, dplat);
            prepareNewLevel(level, renderer, background);
        }
        if(level > 3) break;
        SDL_Delay(TIME_DELAY);
    }

    present(renderer, background, player, plats, dPlats, gPlats, dplat);
    presentScore(renderer, font, textTexture, player.score);
    SDL_Delay(1000);

    if(player.death(dPlats, gPlats, NULL, NULL)) endGame(LOSE, renderer, NULL);
    else
    {
        endGame(WIN, renderer, mWin);
        updRanking(renderer, font, textTexture, player.score);
    }
    presentScore(renderer, font, textTexture, player.score);

    waitUntilKeyPressed();

    releaseMemory(player, plats, gPlats, dPlats, dplat, background,
                  mDead, mGoal, mJump, mNext, mStart, mWin);

    quitSDL(window, renderer);
    return 0;
}
