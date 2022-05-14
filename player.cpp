#include "player.h"

using namespace std;


// player

Player::Player() {}

Player::~Player() {
    /**
    SDL_DestroyTexture(penguinNomal);
    SDL_DestroyTexture(penguinJump);
    SDL_DestroyTexture(penguinFall);
*/
}

Player::Player(int _x, int _y, int _score, SDL_Renderer* renderer): box(_x, _y, 3 * sizeBox / 2, 3 * sizeBox / 2) {
    score = _score;
    loa = 1;
    penguinNomal = loadTexture("picture/penguin/penguinNomal.png", renderer);
    penguinJump = loadTexture("picture/penguin/penguinJump.png", renderer);
    penguinFall = loadTexture("picture/penguin/penguinFall.png", renderer);
 //   penguinLeft = loadTexture("picture/penguin/penguinLeft.png", renderer);
 //   penguinRight = loadTexture("picture/penguin/penguinRight.png", renderer);
}

void Player::render(SDL_Renderer* renderer) {
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

void Player::turn(direct dir, int initSpeed, Mix_Chunk *mState) {
    if(dir == UP && (box.state[UP] || box.state[DOWN])) return;
    box.state[dir] = true;
    box.speed[dir] = initSpeed;
    if(mState != NULL && loa) Mix_PlayChannel( -1, mState, 0 );
}

void Player::prepare() {
    // change direction and speed
    box.state[LEFT] = box.state[RIGHT] = false;

    if(box.state[UP]) {
        box.speed[UP]--;
        if(box.speed[UP] < 0) {
            box.state[UP] = false;
            turn(DOWN, 0, NULL);
        }
    }
    if(box.state[DOWN]) {
        box.speed[DOWN]++;
    }
    else if(!box.state[UP] && fall) turn(DOWN, 0, NULL);
}

bool Player::handle(vector<basicPlat>& plats, destinyPlat& dplat, int& level) {
    box.move();

    Box pre = box.premove();

    fall = ~box.state[UP];

    int n = plats.size();

    // move all plat
    for(int i = 0; i < n; i++) {
        plats[i].move();
        if(i == locate) {
            Box tmp = plats[i].box.premove();
            box.x -= tmp.x - plats[i].box.x;
            box.y -= tmp.y - plats[i].box.y;
        }
    }

    for(int i = 0; i < n; i++) {
        basicPlat plat = plats[i];
        if(box.overlap(plat.box)) fall = 0;

        // hit horizontally
        if(pre.horizontal(plat.box.premove()) == LEFT && box.horizontal(plat.box) != LEFT && box.state[RIGHT]) {
            box.x = min(box.x, plat.box.x - box.width);
        }
        else if(pre.horizontal(plat.box.premove()) == RIGHT && box.horizontal(plat.box) != RIGHT && box.state[LEFT]){
            box.x = max(box.x, plat.box.x + plat.box.width);
        }

        // hit vertically
        if(pre.vertical(plat.box.premove()) == UP && box.vertical(plat.box) != UP && box.state[DOWN] && box.overlap(plat.box)) {
            // falling down hit the plat
            locate = i;
            box.y = plat.box.y - box.height;
            box.state[UP] = box.state[DOWN] = false;
        }
        else if(pre.vertical(plat.box.premove()) == DOWN && box.vertical(plat.box) != DOWN && box.state[UP] && box.overlap(plat.box)) {
            // Can't go up because of hitting plat
            if(box.y < plat.box.y + box.height) {
                box.y = plat.box.y + box.height;
                box.state[UP] = false;
                turn(DOWN, 0, NULL);
            }
        }
    }

    // pass this level
    if(box.overlap(dplat.box)) {
        level++;
        return 1;
    }
    return 0;
}

void Player::keyboardEvent(SDL_Event e, Mix_Chunk *mJump, int& speaker) {
    // keyboard event
    switch (e.key.keysym.sym) {
        case SDLK_UP: turn(UP, MAXSPEED, mJump); break;
//        case SDLK_DOWN: moveDown(); break;
        case SDLK_RIGHT: turn(RIGHT, sizeBox, NULL); break;
        case SDLK_LEFT: turn(LEFT, sizeBox, NULL); break;
        case SDLK_p: speaker = 1 - speaker; loa = speaker; break;
        default: break;
    }
}

bool Player::death(vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, Mix_Chunk *mDead, Mix_Chunk *mGoal) {
    if (box.fallOut()) {
            // out of map
        if(mDead != NULL && loa) Mix_PlayChannel( -1, mDead, 0 );
        return 1;
    }
    for(deadPlat plat : dPlats) if(box.overlap(plat.box)) {
        // hit the dead plat
        if(mDead != NULL && loa) Mix_PlayChannel( -1, mDead, 0 );
        return 1;
    }
    for(goalPlat &plat : gPlats) if(plat.exist && box.overlap(plat.box)) {
        // collect coin
        score++;
        if(mDead != NULL && loa) Mix_PlayChannel( -1, mGoal, 0 );
        plat.exist = false;
    }
    return 0;
}

