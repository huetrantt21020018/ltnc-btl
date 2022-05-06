#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>

#include "common.h"
#include "mainGame.h"
#include "SDL_utils.h"

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
    if(x + width <= o.x) return LEFT;
    if(x >= o.x+o.width) return RIGHT;
    return NONE;
}
direct Box::vertical(Box o)
{
    if(!(max(x, o.x) < min(x+width, o.x+o.width))) return NONE;
    if(y + height <= o.y) return UP;
    if(y >= o.y+o.height) return DOWN;
    return NONE;
}

// player

Player::Player() {}
Player::Player(int _x, int _y, int _score, SDL_Renderer* renderer): box(_x, _y, 3 * sizeBox / 2, 3 * sizeBox / 2) {
    score = _score;
    penguinNomal = loadTexture("penguinNomal.png", renderer);
    penguinJump = loadTexture("penguinJump.png", renderer);
    penguinFall = loadTexture("penguinFall.png", renderer);
 //   penguinLeft = loadTexture("penguinLeft.png", renderer);
 //   penguinRight = loadTexture("penguinRight.png", renderer);
}

void Player::render(SDL_Renderer* renderer)
{
    // box.render(renderer, 8, 99, 255, 26);
    SDL_Texture* penguin = penguinNomal;
    if(box.state[UP]) penguin = penguinJump;
    else if(box.state[DOWN]) penguin = penguinFall;
    //else if(box.state[LEFT]) penguin = penguinLeft;
    //else if(box.state[RIGHT]) penguin = penguinRight;

    SDL_Rect penRect;
    penRect.x = box.x;
    penRect.y = box.y;
    penRect.w = box.width;
    penRect.h = box.height;

    SDL_RenderCopy(renderer, penguin, NULL, &penRect);
}

void Player::turn(direct dir, int initSpeed)
{
    if(dir == UP && (box.state[UP] || box.state[DOWN])) return;
    box.state[dir] = true;
    box.speed[dir] = initSpeed;
}

void Player::prepare()
{
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

bool Player::handle(vector<basicPlat>& plats, destinyPlat& dplat, int& level)
{
    box.move();

    Box pre = box.premove();

    fall = ~box.state[UP];

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
            box.x = min(box.x, plat.box.x - box.width);
        }
        else if(pre.horizontal(plat.box) == RIGHT && box.horizontal(plat.box) != RIGHT && box.state[LEFT])
        {
            box.x = max(box.x, plat.box.x + plat.box.width);
        }

        if(pre.vertical(plat.box) == UP && box.vertical(plat.box) != UP && box.state[DOWN] && box.overlap(plat.box))
        {
            locate = i;
            box.y = plat.box.y - box.height;
            box.state[UP] = box.state[DOWN] = false;
        }
        else if(pre.vertical(plat.box) == DOWN && box.vertical(plat.box) != DOWN && box.state[UP] && box.overlap(plat.box))
        {
            if(box.y < plat.box.y + box.height)
            {
                box.y = plat.box.y + box.height;
                box.state[UP] = false;
                turn(DOWN, 0);
            }
        }
    }

    if(box.overlap(dplat.box))
    {
        level++;
        return 1;
    }
    return 0;
}

void Player::keyboardEvent(SDL_Event e)
{
    // keyboard event
    switch (e.key.keysym.sym)
    {
        case SDLK_UP: turn(UP, MAXSPEED); break;
//        case SDLK_DOWN: moveDown(); break;
        case SDLK_RIGHT: turn(RIGHT, sizeBox); break;
        case SDLK_LEFT: turn(LEFT, sizeBox); break;
        default: break;
    }
}

bool Player::death(vector<deadPlat>& dPlats, vector<goalPlat>& gPlats)
{
    if (box.fallOut()) return 1;
    for(deadPlat plat : dPlats) if(box.overlap(plat.box)) return 1;
    for(goalPlat &plat : gPlats) if(plat.exist && box.overlap(plat.box))
    {
        score++;
        cout << score << endl;
        plat.exist = false;
    }
    return 0;
}

// basicPlat

basicPlat::basicPlat() {}
basicPlat::basicPlat(int _x, int _y, int length, direct _dir):
    box(_x, _y, length, sizeBox) {box.state[_dir] = 1; dir = _dir;}

void basicPlat::render(SDL_Renderer* renderer)
{
    box.render(renderer, 157, 110, 40, 15);
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

// goal Plat

goalPlat::goalPlat() {}
goalPlat::goalPlat(int _x, int _y, SDL_Renderer* renderer) : box(_x, _y, sizeBox, sizeBox)
{
    glPlat = loadTexture("coin.jpg", renderer);
}

void goalPlat::render(SDL_Renderer* renderer)
{
//    box.render(renderer, 252, 103, 99, 56);
    SDL_Texture* plat = glPlat;

    SDL_Rect platRect;
    SDL_QueryTexture(plat, NULL, NULL, &platRect.w, &platRect.h);
    platRect.x = box.x;
    platRect.y = box.y;
    platRect.w = box.width;
    platRect.h = box.height;

    SDL_RenderCopy(renderer, plat, NULL, &platRect);
}

// destiny Plat

destinyPlat::destinyPlat() {}
destinyPlat::destinyPlat(int _x, int _y, SDL_Renderer* renderer) : box(_x, _y, sizeBox, sizeBox)
{
    desPlat = loadTexture("tornado.jpg", renderer);
}

void destinyPlat::render(SDL_Renderer* renderer)
{
//    box.render(renderer, 252, 103, 99, 56);
    SDL_Texture* plat = desPlat;

    SDL_Rect platRect;
    SDL_QueryTexture(plat, NULL, NULL, &platRect.w, &platRect.h);
    platRect.x = box.x;
    platRect.y = box.y;
    platRect.w = box.width;
    platRect.h = box.height;

    SDL_RenderCopy(renderer, plat, NULL, &platRect);
}


// dead Plat

deadPlat::deadPlat() {}
deadPlat::deadPlat(int _x, int _y, int length, SDL_Renderer* renderer) : box(_x, _y, length, sizeBox)
{
    DeadPlat = loadTexture("deadPlat.jpg", renderer);
}

void deadPlat::render(SDL_Renderer* renderer)
{
    // box.render(renderer, 8, 99, 255, 26);
    SDL_Texture* plat = DeadPlat;

    SDL_Rect platRect;
    SDL_QueryTexture(plat, NULL, NULL, &platRect.w, &platRect.h);
    platRect.x = box.x;
    platRect.y = box.y;
    platRect.w = box.width;
    platRect.h = box.height;

    SDL_RenderCopy(renderer, plat, NULL, &platRect);
}

// game handling

void prepareNewLevel(int level, SDL_Renderer* renderer, SDL_Texture* &background)
{
    string s1 = "background";
    string s2 = to_string(level);
    string s3 = ".png";
    string fileName = s1 + s2 + s3;

    background = loadTexture(fileName, renderer);

    s1 = "signLevel";
    fileName = s1 + s2 + s3;
    SDL_Texture* sign = loadTexture(fileName, renderer);

    SDL_Rect signRect;
    SDL_QueryTexture(sign, NULL, NULL, &signRect.w, &signRect.h);
    signRect.x = 75;
    signRect.y = 0;

    SDL_RenderCopy(renderer, sign, NULL, &signRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
}

void initGame(Player& player, vector<basicPlat>& plats, vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, destinyPlat& dplat, int level, SDL_Renderer* renderer)
{
    //srand(time(nullptr));

    player = Player((SCREEN_WIDTH + 5*sizeBox)/2, initialHeight, player.score, renderer);

    string s1 = "level";
    string s2 = to_string(level);
    string s3 = ".txt";

    string fileName = s1 + s2 + s3;
    ifstream inFile(fileName);

    int n;
    inFile >> n;

    plats.clear();
    for(int i = 0; i < n; ++i)
    {
        int x, y, len, d;
        direct dir;
        inFile >> x >> y >> len >> d;
        dir = direct(d);
        basicPlat plat(x, y, len, dir);
        plats.push_back(plat);
    }

    dPlats.clear();
    inFile >> n;
    for(int i = 0; i < n; ++i)
    {
        int x, y, len;
        inFile >> x >> y >> len;
        deadPlat plat(x, y, len, renderer);
        dPlats.push_back(plat);
    }

    gPlats.clear();
    inFile >> n;
    for(int i = 0; i < n; ++i)
    {
        int x, y;
        inFile >> x >> y;
        goalPlat plat(x, y, renderer);
        gPlats.push_back(plat);
    }

    int x, y, len;
    inFile >> x >> y >> len;
    dplat = destinyPlat(x, y, renderer);
}

void present(SDL_Renderer* renderer, SDL_Texture* background, Player &player, vector<basicPlat>& plats, vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, destinyPlat& dplat)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, background, NULL, NULL);

    for(basicPlat plat : plats) plat.render(renderer);
    for(deadPlat plat : dPlats) plat.render(renderer);
    for(goalPlat plat : gPlats) if(plat.exist) plat.render(renderer);
    dplat.render(renderer);

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

void endGame(game Game, SDL_Renderer* renderer)
{
    SDL_Texture* sign = loadTexture("signStart.png", renderer);
    if(Game == LOSE) sign =  loadTexture("signFail.png", renderer);
    if(Game == WIN) sign = loadTexture("signWin.png", renderer);

    SDL_Rect signRect;
    SDL_QueryTexture(sign, NULL, NULL, &signRect.w, &signRect.h);
    signRect.x = 75;
    signRect.y = SCREEN_HEIGHT - signRect.h + 20;

    SDL_RenderCopy(renderer, sign, NULL, &signRect);
    SDL_RenderPresent(renderer);

    SDL_Event e;
    do {
        if(SDL_WaitEvent(&e) != 0 && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
            break;
        SDL_Delay(100);
    } while(true);
}
