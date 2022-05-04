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
//    TTF_Font* font = NULL;
//    LTexture textTexture;

    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    // init game

    Player player((SCREEN_WIDTH + 5*sizeBox)/2, initialHeight);
    vector<basicPlat> plats;
    vector<deadPlat> dPlats;

    int level = 1;
    initGame(plats, dPlats, level);

    // game loop

    while(!player.death(dPlats)) {
        present(renderer, player, plats, dPlats, level);
        player.prepare();
        if(keyboardEvent(player)) break;
        player.handle(plats);
        SDL_Delay(TIME_DELAY);
    }
    present(renderer, player, plats, dPlats, level);

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
