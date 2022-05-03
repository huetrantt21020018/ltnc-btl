#include "mainGame.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>

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
   // cout << x << ' ' << y << endl;
    SDL_Rect filled_rect;
    filled_rect.x = x;
    filled_rect.y = y;
    filled_rect.w = width;
    filled_rect.h = height;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &filled_rect);
}

int Box::move()
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
    if(preX == x && preY == y) return 0;
    return 1;
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
    return (max(x, o.x) <= min(x+width, o.x+o.width)
            && max(y, o.y) <= min(y+height, o.y+o.height));
}

direct Box::horizontal(Box o)
{
    if(!(max(y, o.y) < min(y+height, o.y+o.height))) return NONE;
    if(x + sizeBox <= o.x) return LEFT;
    if(x >= o.x+o.width) return RIGHT;
    return NONE;
}
direct Box::vertical(Box o)
{
    if(!(max(x, o.x) < min(x+width, o.x+o.width))) return NONE;
    if(y + sizeBox <= o.y) return UP;
    if(y >= o.y+o.height) return DOWN;
    return NONE;
}

// player

Player::Player() {}
Player::Player(int _x, int _y):
    box(_x, _y, sizeBox, sizeBox) {}

void Player::render(SDL_Renderer* renderer)
{
    //cout << box.state[UP] << ' ' << box.state[DOWN] << ' ' << box.state[LEFT] << ' ' << box.state[RIGHT] << endl;
    box.render(renderer, 8, 99, 255, 26);
}

void Player::turn(direct dir, int initSpeed)
{
    if(dir == UP && (box.state[UP] || box.state[DOWN])) return;
    box.state[dir] = true;
    box.speed[dir] = initSpeed;
}

void Player::handle(vector<basicPlat>& plats)
{
    box.move();
    Box pre = box.premove();

    bool fall = ~box.state[UP];

    int n = plats.size();

    for(int i = 0; i < n; i++)
    {
        plats[i].move();
        if(i == locate)
        {
            Box tmp = plats[i].box.premove();
            box.x -= tmp.x - plats[i].box.x;
            box.y -= tmp.y - plats[i].box.y;
        }
    }
    for(int i = 0; i < n; i++)
    {
        basicPlat plat = plats[i];
        if(box.overlap(plat.box)) fall = 0;

        if(pre.horizontal(plat.box) == LEFT && box.horizontal(plat.box) != LEFT && box.state[RIGHT])
        {
            box.x = min(box.x, plat.box.x - sizeBox);
        }
        else if(pre.horizontal(plat.box) == RIGHT && box.horizontal(plat.box) != RIGHT && box.state[LEFT])
        {
            box.x = max(box.x, plat.box.x + plat.box.width);
        }

        if(pre.vertical(plat.box) == UP && box.vertical(plat.box) != UP && box.state[DOWN] && box.overlap(plat.box))
        {
            locate = i;
            box.y = plat.box.y - sizeBox;
            box.state[UP] = box.state[DOWN] = false;
        }
        else if(pre.vertical(plat.box) == DOWN && box.vertical(plat.box) != DOWN && box.state[UP] && box.overlap(plat.box))
        {
            if(box.y < plat.box.y + sizeBox)
            {
                box.y = plat.box.y + sizeBox;
                box.state[UP] = false;
                turn(DOWN, 0);
            }
        }
    }

    box.state[LEFT] = box.state[RIGHT] = false;

    if(box.state[UP])
    {
        box.speed[UP]--;
        if(box.speed[UP] < 0)
        {
            box.state[UP] = false;
            turn(DOWN, 0);
        }
    }
    if(box.state[DOWN])
    {
        box.speed[DOWN]++;
    }
    else if(!box.state[UP] && fall) turn(DOWN, 0);

}

void Player::keyboardEvent(SDL_Event e)
{
    // keyboard event
    switch (e.key.keysym.sym)
    {
        case SDLK_UP: turn(UP, MAXSPEED); break;
//        case SDLK_DOWN: moveDown(); break;
        case SDLK_RIGHT: turn(RIGHT, sizeBox - 1); break;
        case SDLK_LEFT: turn(LEFT, sizeBox - 1); break;
        default: break;
    }
}

bool Player::death()
{
    if (box.fallOut()) return 1;
    return 0;
}

// basicPlat

basicPlat::basicPlat() {}
basicPlat::basicPlat(int _x, int _y, int length, direct _dir):
    box(_x, _y, length, sizeBox) {box.state[_dir] = 1; dir = _dir;}

void basicPlat::render(SDL_Renderer* renderer)
{
    box.render(renderer, 33, 187, 114, -52);
}

void basicPlat::move()
{
    //cout << "# " << box.x << ' ' << box.y << endl;
    if(!box.move())
    {
        box.state[dir] = false;
        dir = opposite[dir];
        box.state[dir] = true;
    }
    //cout << "## " << box.x << ' ' << box.y << endl;
}

// game handling

void initGame(vector<basicPlat>& plats, int level)
{
    //srand(time(nullptr));

    string s1 = "level";
    string s2 = to_string(level);
    string s3 = ".txt";

    string fileName = s1 + s2 + s3;
    cout << fileName << endl;
    ifstream inFile(fileName);

    int n;
    inFile >> n;

    for(int i = 0; i < n; ++i)
    {
        int x, y, len, d;
        direct dir;
        inFile >> x >> y >> len >> d;
        dir = direct(d);
        cout << dir << endl;
        basicPlat plat(x, y, len, dir);
        plats.push_back(plat);
    }
}

void present(SDL_Renderer* renderer, Player &player, vector<basicPlat>& plats)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    player.render(renderer);
    for(basicPlat plat : plats) plat.render(renderer);

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
