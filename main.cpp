#include <iostream>
#include <SDL.h>

#include "common.h"
#include "SDL_utils.h"
#include "mainGame.h"

using namespace std;

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    // Your drawing code here
    // use SDL_RenderPresent(renderer) to show it

    Player player(SCREEN_WIDTH/2, initialHeight);

    while(!player.death()) {
        present(renderer, player);
        if(keyboardEvent(player)) break;
        player.handle();
        SDL_Delay(TIME_DELAY);
    }

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
