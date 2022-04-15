#include "mainGame.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "common.h"
#include "mainGame.h"

using namespace std;

int rnd(int a, int b) { return (rand() % (b-a+1) + a); }

// box

Box::Box() {}
Box::Box(int _x, int _y, int _w, int _h):
    x(_x), y(_y), width(_w), height(_h) {}

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
    preX = x;
    preY = y;
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

Box Box::premove()
{
    Box ret(preX, preY, width, height);
    return ret;
}

bool Box::fallOut()
{
    return y + height >= SCREEN_HEIGHT;
}

bool Box::insideScreen()
{
    return (0 <= x && x + width <= SCREEN_WIDTH
            && 0 <= y && y + height <= SCREEN_HEIGHT);
}

bool Box::overlap(Box o)
{
    return (max(x, o.x) < min(x+width, o.x+o.width)
            && max(y, o.y) < min(y+height, o.y+o.height));
}

direct Box::horizontal(Box o)
{
    if(x < o.x && x+width > o.x) return LEFT;
    if(x > o.x && x < o.x+o.width) return RIGHT;
    return NONE;
}
direct Box::vertical(Box o)
{
    if(y < o.y && y+height > o.y) return UP;
    if(y > o.y && y < o.y+o.height) return DOWN;
    return NONE;
}

// player

Player::Player() {}
Player::Player(int _x, int _y):
    box(_x, _y, sizeBox, sizeBox) {}

void Player::render(SDL_Renderer* renderer)
{
    cout << box.x << ' ' << box.y << endl;
    box.render(renderer, 8, 99, 255, 26);
}

void Player::turn(direct dir, int initSpeed)
{
    if(dir == UP && (box.state[UP] || box.state[DOWN])) return;
    box.state[dir] = true;
    box.speed[dir] = initSpeed;
}

void Player::handle(vector<PlatBasic>& plats)
{
    box.move();
    Box pre = box.premove();

    /// BUG HERE

    for(PlatBasic plat : plats) if(box.overlap(plat.box))
    {
        if(pre.horizontal(plat.box) == LEFT)
        {
            box.x = plat.box.x - sizeBox;
        }
        else if(pre.horizontal(plat.box) == RIGHT)
        {
            box.x = plat.box.x + plat.box.width;
        }

        if(pre.vertical(plat.box) == UP)
        {
            box.y = plat.box.y - sizeBox;
            box.state[UP] = box.state[DOWN] = false;
        }
        else if(pre.vertical(plat.box) == DOWN)
        {
            box.y = plat.box.y + sizeBox;
            box.state[UP] = false;
            box.state[DOWN] = true;
            box.speed[DOWN] = 0;
        }
        break;
    }

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
PlatBasic::PlatBasic(int _x, int _y, int length):
    box(_x, _y, length, sizeBox) {}

void PlatBasic::render(SDL_Renderer* renderer)
{
    box.render(renderer, 33, 187, 114, -52);
}

// game handling

void initGame(vector<PlatBasic>& plats)
{
    srand(time(nullptr));

    int len = 60;
    int x = SCREEN_WIDTH/2 - len/2 + sizeBox/2;
    int y = initialHeight + sizeBox;
    PlatBasic plat(x, y, len);
    plats.push_back(plat);

    int pre = y;

    for(int i = 1; i < 10; i++)
    {
        int len = rnd(50, 100);
        int x = rnd(0, SCREEN_WIDTH - len);
        int y = rnd(max(sizeBox, pre - 100), pre - sizeBox);
        PlatBasic plat(x, y, len);
        plats.push_back(plat);
        pre = y;
    }
}

void present(SDL_Renderer* renderer, Player &player, vector<PlatBasic>& plats)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    player.render(renderer);
    for(PlatBasic plat : plats) plat.render(renderer);

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

void handleGame(Player &player, vector<PlatBasic> &plats)
{

}
