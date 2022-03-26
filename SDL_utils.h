#ifndef SDL_UTILS__H_
#define SDL_UTILS__H_

#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"

using namespace std;


void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT, const string WINDOW_TITLE);

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void waitUntilKeyPressed();

#endif // SDL_UTILS
