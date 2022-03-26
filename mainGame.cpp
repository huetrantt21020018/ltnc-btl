#include "mainGame.h"
#include <iostream>

#include "common.h"
#include "mainGame.h"

using namespace std;

// box

Box::Box() {}
Box::Box(int _x, int _y, int _w, int _h): x(_x), y(_y), width(_w), height(_h) {}

void Box::render(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Rect filled_rect;
    filled_rect.x = x;
    filled_rect.y = y;
    filled_rect.w = width;
    filled_rect.h = height;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &filled_rect);
}

void Box::move()
{
    for(int dir = UP; dir <= RIGHT; dir = dir + 1) if(state[dir])
    {
        x += stepX[dir] * speed[dir];
        y += stepY[dir] * speed[dir];

        // outside
        x = max(x, 0);
        y = max(y, 0);
        x = min(x, SCREEN_WIDTH - width);
        y = min(y, SCREEN_HEIGHT - height);
    }
}

bool Box::fallOut()
{
    return y + height >= SCREEN_HEIGHT;
}

bool Box::insideScreen()
{
    return 0 <= x && x + width <= SCREEN_WIDTH && 0 <= y && y + height <= SCREEN_HEIGHT;
}

// player

Player::Player() {}
Player::Player(int _x, int _y): box(_x, _y, sizeBox, sizeBox) {}

void Player::render(SDL_Renderer* renderer)
{
    box.render(renderer, 8, 99, 255, 26);
}

void Player::turn(direct dir, int initSpeed)
{
    if(dir == UP && (box.state[UP] || box.state[DOWN])) return;
    box.state[dir] = true;
    box.speed[dir] = initSpeed;
}

void Player::handle()
{
    box.move();

    box.state[LEFT] = box.state[RIGHT] = false;
    if(box.state[UP])
    {
        box.speed[UP]--;
        if(box.speed[UP] < 0)
        {
            box.state[UP] = false;
            box.state[DOWN] = true;
            box.speed[DOWN] = 0;
        }
    }
    if(box.state[DOWN])
    {
        box.speed[DOWN]++;
        if(box.speed[DOWN] > MAXSPEED)
        {
            box.state[DOWN] = false;
        }
    }
}

void Player::keyboardEvent(SDL_Event e)
{
    // keyboard event
    switch (e.key.keysym.sym)
    {
        case SDLK_UP: turn(UP, MAXSPEED); break;
//        case SDLK_DOWN: moveDown(); break;
        case SDLK_RIGHT: turn(RIGHT, MAXSPEED/3); break;
        case SDLK_LEFT: turn(LEFT, MAXSPEED/3); break;
        default: break;
    }
}

bool Player::death()
{
    if (box.fallOut()) return 1;
    return 0;
}

// PlatBasic

PlatBasic::PlatBasic() {}
PlatBasic::PlatBasic(int _x, int _y, int length): box(_x, _y, sizeBox, length) {}

void PlatBasic::render(SDL_Renderer* renderer)
{
    box.render(renderer, 0, 0, 0, 255);
}

// game handling

void present(SDL_Renderer* renderer, Player &player)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    player.render(renderer);
    SDL_RenderPresent(renderer);
}

bool keyboardEvent(Player& player)
{
    SDL_Event e;
    if(SDL_PollEvent(&e) == 0) return 0;
    if(e.type == SDL_QUIT) return 1;
    if(e.type == SDL_KEYDOWN)
        player.keyboardEvent(e);
    return 0;
}
