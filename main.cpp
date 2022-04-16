#include <SDL.h>
#include <iostream>
#include <vector>

#include "common.h"
#include "SDL_utils.h"
#include "mainGame.h"

using namespace std;

int main(int argc, char* argv[]) {

    // init SDL

    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    // init game

    Player player(SCREEN_WIDTH/2, initialHeight);
    vector<PlatBasic> plats;
    initGame(plats);

    // game loop

    while(!player.death()) {
        present(renderer, player, plats);
        if(keyboardEvent(player)) break;
        player.handle(plats);
//        handleGame(player, plats);
        SDL_Delay(TIME_DELAY);
    }

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
