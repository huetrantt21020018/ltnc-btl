#include <SDL.h>
#include <SDL_ttf.h>

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


    // init game

    Player player((SCREEN_WIDTH + 5*sizeBox)/2, initialHeight, 0, renderer);
    vector<basicPlat> plats;
    vector<deadPlat> dPlats;
    vector<goalPlat> gPlats;
    destinyPlat dplat;

    int level = 1;
    initGame(player, plats, dPlats, gPlats, dplat, level, renderer);

    SDL_Texture* background = loadTexture("picture/background/background1.png", renderer);

    present(renderer, background, player, plats, dPlats, gPlats, dplat);
    endGame(START, renderer);
    present(renderer, background, player, plats, dPlats, gPlats, dplat);
    prepareNewLevel(level, renderer, background);

    // game loop

    while(!player.death(dPlats, gPlats)) {
        present(renderer, background, player, plats, dPlats, gPlats, dplat);
        presentScore(renderer, font, textTexture, player.score);
        player.prepare();
        if(keyboardEvent(player)) break;
        if(player.handle(plats, dplat, level) && level <= 3)
        {
            initGame(player, plats, dPlats, gPlats, dplat, level, renderer);
            present(renderer, background, player, plats, dPlats, gPlats, dplat);
            prepareNewLevel(level, renderer, background);
        }
        if(level > 1) break;
        SDL_Delay(TIME_DELAY);
    }

    present(renderer, background, player, plats, dPlats, gPlats, dplat);
    presentScore(renderer, font, textTexture, player.score);
    SDL_Delay(1000);

    if(player.death(dPlats, gPlats)) endGame(LOSE, renderer);
    else
    {
        endGame(WIN, renderer);
        updRanking(renderer, font, textTexture, player.score);
    }
    presentScore(renderer, font, textTexture, player.score);

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
