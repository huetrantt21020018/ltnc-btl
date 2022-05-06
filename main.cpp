#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>
#include <vector>

#include "common.h"
#include "SDL_utils.h"
#include "mainGame.h"
#include "text_object.h"

using namespace std;

int main(int argc, char* argv[]) {

    // init SDL

    SDL_Window* window;
    SDL_Renderer* renderer;

    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    // init game

    Player player((SCREEN_WIDTH + 5*sizeBox)/2, initialHeight, renderer);
    vector<basicPlat> plats;
    vector<deadPlat> dPlats;
    goalPlat gplat;

    int level = 1;
    initGame(player, plats, dPlats, gplat, level, renderer);

    SDL_Texture* background = loadTexture("background1.png", renderer);

    present(renderer, background, player, plats, dPlats, gplat);
    endGame(START, renderer);
    present(renderer, background, player, plats, dPlats, gplat);
    prepareNewLevel(level, renderer, background);

    // game loop

    while(!player.death(dPlats)) {
        present(renderer, background, player, plats, dPlats, gplat);
        player.prepare();
        if(keyboardEvent(player)) break;
        if(player.handle(plats, gplat, level) && level <= 3)
        {
            initGame(player, plats, dPlats, gplat, level, renderer);
            present(renderer, background, player, plats, dPlats, gplat);
            prepareNewLevel(level, renderer, background);
        }
        if(level > 3) break;
        SDL_Delay(TIME_DELAY);
    }

    present(renderer, background, player, plats, dPlats, gplat);
    SDL_Delay(1000);

    if(player.death(dPlats)) endGame(LOSE, renderer);
    else endGame(WIN, renderer);

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
