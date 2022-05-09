#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

#include "common.h"
#include "mainGame.h"
#include "SDL_utils.h"
#include "SDL_text.h"

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
    penguinNomal = loadTexture("picture/penguin/penguinNomal.png", renderer);
    penguinJump = loadTexture("picture/penguin/penguinJump.png", renderer);
    penguinFall = loadTexture("picture/penguin/penguinFall.png", renderer);
 //   penguinLeft = loadTexture("picture/penguin/penguinLeft.png", renderer);
 //   penguinRight = loadTexture("picture/penguin/penguinRight.png", renderer);
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
    glPlat = loadTexture("picture/icon/coin.jpg", renderer);
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
    desPlat = loadTexture("picture/icon/tornado.jpg", renderer);
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
    DeadPlat = loadTexture("picture/icon/deadPlat.jpg", renderer);
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
    string s1 = "picture/background/background";
    string s2 = to_string(level);
    string s3 = ".png";
    string fileName = s1 + s2 + s3;

    background = loadTexture(fileName, renderer);

    s1 = "picture/sign/signLevel";
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

    string s1 = "map/level";
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

void presentScore(SDL_Renderer* renderer, TTF_Font* font, LTexture textTexture, int score)
{
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

    SDL_Color textColor = { 0, 0, 0 };
    string s1 = "SCORE: ";
    string s2 = to_string(score);
    string text = s1 + s2;
    if( !textTexture.loadFromRenderedText( text, textColor, renderer, font ) )
    {
        printf( "Failed to render text texture!\n" );
        return;
    }

    textTexture.render(sizeBox, sizeBox, renderer);
    SDL_RenderPresent(renderer);

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

    //SDL_RenderPresent(renderer);
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
    SDL_Texture* sign = loadTexture("picture/sign/signStart.png", renderer);
    if(Game == LOSE) sign =  loadTexture("picture/sign/signFail.png", renderer);
    if(Game == WIN) sign = loadTexture("picture/sign/signWin.png", renderer);

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

void updRanking(SDL_Renderer* renderer, TTF_Font* font, LTexture textTexture, int score)
{
    // upd file ranking

    ifstream inFile("ranking.txt");

    vector<int> scores;
    scores.push_back(score);

    int oScore;
    while(inFile >> oScore)
        scores.push_back(oScore);

    sort(scores.begin(), scores.end(), greater<int> ());
    if(scores.size() > 5) scores.pop_back();

    ofstream outFile("ranking.txt");
    for(int x: scores) outFile << x << endl;

    // in ra man hinh
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_Color textColor = { 255, 255, 255 };

    string s1 = "HIGHEST SCORE: ";
    string s2 = to_string(*scores.begin());
    string text = s1 + s2;
    if(score == *scores.begin() && score > *next(scores.begin()))
        text = "NEW HIGHEST SCORE!";

    if( !textTexture.loadFromRenderedText( text, textColor, renderer, font ) )
    {
        printf( "Failed to render text texture!\n" );
        return;
    }
    textTexture.render(205, 575, renderer);
    SDL_RenderPresent(renderer);

}
