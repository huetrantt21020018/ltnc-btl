#include "mainGame.h"

using namespace std;

int rnd(int a, int b) {
    return (rand() % (b-a+1) + a);
}

// game handling

void prepareNewLevel(int level, SDL_Renderer* renderer, SDL_Texture* &background) {

    // load background and sign

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
    signRect.x = 70;
    signRect.y = 0;

    SDL_RenderCopy(renderer, sign, NULL, &signRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
}

void initGame(Player& player, vector<basicPlat>& plats, vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, destinyPlat& dplat, int level, SDL_Renderer* renderer, Mix_Chunk *mStart) {
    //srand(time(nullptr));

    player = Player((SCREEN_WIDTH + 5*sizeBox)/2, initialHeight, player.score, renderer);
    free(plats, gPlats, dPlats, dplat);

    string s1 = "map/level";
    string s2 = to_string(level);
    string s3 = ".txt";

    string fileName = s1 + s2 + s3;
    ifstream inFile(fileName);

    // input basic plat

    int n;
    inFile >> n;

    for(int i = 0; i < n; ++i) {
        int x, y, len, d;
        direct dir;
        inFile >> x >> y >> len >> d;
        dir = direct(d);

        basicPlat plat(x, y, len, dir);
        plats.push_back(plat);
    }

    // input dead plat

    inFile >> n;
    for(int i = 0; i < n; ++i) {
        int x, y, len;
        inFile >> x >> y >> len;
        deadPlat plat(x, y, len, renderer);
        dPlats.push_back(plat);
    }

    // input goal plat

    inFile >> n;
    for(int i = 0; i < n; ++i) {
        int x, y;
        inFile >> x >> y;
        goalPlat plat(x, y, renderer);
        gPlats.push_back(plat);
    }

    // input destiny plat

    int x, y, len;
    inFile >> x >> y >> len;
    dplat = destinyPlat(x, y, renderer);

    // play music

    Mix_PlayChannel( -1, mStart, 0 );
}

void presentScore(SDL_Renderer* renderer, TTF_Font* font, LTexture textTexture, int score, int speaker) {
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

    SDL_Color textColor = { 0, 0, 0 };
    string s1 = "SCORE: ";
    string s2 = to_string(score);
    string text = s1 + s2;
    if( !textTexture.loadFromRenderedText( text, textColor, renderer, font ) ) {
        printf( "Failed to render text texture!\n" );
        return;
    }

    textTexture.render(sizeBox, sizeBox, renderer);

    s1 = "speaker: ";
    s2 = speaker == 0 ? "off" : "on";
    text = s1 + s2;

    if( !textTexture.loadFromRenderedText( text, textColor, renderer, font ) ) {
        printf( "Failed to render text texture!\n" );
        return;
    }

    textTexture.render(sizeBox, 3 * sizeBox, renderer);
    SDL_RenderPresent(renderer);

}

void present(SDL_Renderer* renderer, SDL_Texture* background, Player &player, vector<basicPlat>& plats, vector<deadPlat>& dPlats, vector<goalPlat>& gPlats, destinyPlat& dplat) {
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

bool keyboardEvent(Player& player, Mix_Chunk* mState, int& speaker) {
    SDL_Event e;
    if(SDL_PollEvent(&e) == 0) return 0;
    if(e.type == SDL_QUIT) return 1;
    if(e.type == SDL_KEYDOWN)
        player.keyboardEvent(e, mState, speaker);
    return 0;
}

void endGame(game Game, SDL_Renderer* renderer, Mix_Chunk *mState, int speaker) {
    SDL_Texture* sign = loadTexture("picture/sign/signStart.png", renderer);
    if(Game == LOSE) sign =  loadTexture("picture/sign/signFail.png", renderer);
    if(Game == WIN) sign = loadTexture("picture/sign/signWin.png", renderer);

    SDL_Rect signRect;
    SDL_QueryTexture(sign, NULL, NULL, &signRect.w, &signRect.h);
    signRect.x = 70;
    signRect.y = SCREEN_HEIGHT - signRect.h + 20;

    if(mState != NULL && speaker) Mix_PlayChannel( -1, mState, 0 );

    SDL_RenderCopy(renderer, sign, NULL, &signRect);
    SDL_RenderPresent(renderer);


    // exit game
    SDL_Event e;
    do {
        if(SDL_WaitEvent(&e) != 0 && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
            break;
        SDL_Delay(100);
    } while(true);
}

void updRanking(SDL_Renderer* renderer, TTF_Font* font, LTexture textTexture, int score) {
    // update file ranking

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

    // present screen
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_Color textColor = { 255, 255, 255 };

    string s1 = "HIGHEST SCORE: ";
    string s2 = to_string(*scores.begin());
    string text = s1 + s2;
    if(score == *scores.begin() && score > *next(scores.begin()))
        text = "NEW HIGHEST SCORE!";

    if( !textTexture.loadFromRenderedText( text, textColor, renderer, font ) ) {
        printf( "Failed to render text texture!\n" );
        return;
    }
    textTexture.render(205, 575, renderer);
    SDL_RenderPresent(renderer);
}

void free(vector<basicPlat> &plats, vector<goalPlat> &gPlats, vector<deadPlat> &dPlats, destinyPlat &dplat) {

    plats.clear();

    for(goalPlat &plat: gPlats) SDL_DestroyTexture(plat.glPlat);
    gPlats.clear();

    for(deadPlat &plat: dPlats) SDL_DestroyTexture(plat.DeadPlat);
    dPlats.clear();

    SDL_DestroyTexture(dplat.desPlat);
}

void releaseMemory(Player &player, vector<basicPlat> &plats, vector<goalPlat> &gPlats, vector<deadPlat> &dPlats, destinyPlat &dplat, SDL_Texture* &background,
                  Mix_Chunk* &mDead, Mix_Chunk* &mGoal, Mix_Chunk* &mJump, Mix_Chunk* &mNext, Mix_Chunk* &mStart, Mix_Chunk* &mWin, Mix_Music* &mBeat) {

    SDL_DestroyTexture(player.penguinNomal);
    SDL_DestroyTexture(player.penguinNomal);
    SDL_DestroyTexture(player.penguinNomal);

    free(plats, gPlats, dPlats, dplat);
    SDL_DestroyTexture(background);

    Mix_FreeChunk(mDead);
    Mix_FreeChunk(mGoal);
    Mix_FreeChunk(mJump);
    Mix_FreeChunk(mNext);
    Mix_FreeChunk(mStart);
    Mix_FreeChunk(mWin);

    Mix_FreeMusic(mBeat);
}

bool checkInside(int x, int y, SDL_Rect rect)
{
    return x >= rect.x && x <= rect.x + rect.w
            && y >= rect.y && y <= rect.y + rect.h;
}

int makeMenu(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* background) {
    const int MENU_ITEM = 2;
    LTexture textMenu[MENU_ITEM];
    bool selectedMenu[MENU_ITEM] = {0, 0};
    std::string labels[MENU_ITEM] = {"NEW GAME", "EXIT"};

    for (int i = 0; i < MENU_ITEM; ++i)
        textMenu[i].loadFromRenderedText(labels[i], {0, 0, 255}, renderer, font);

    int x = 0, y = 0;

    SDL_Event e;
    for (;;) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        for (int i = 0; i < MENU_ITEM; ++i) {
            int X = (SCREEN_WIDTH - textMenu[i].getWidth()) / 2;
            int Y = (SCREEN_HEIGHT - textMenu[i].getHeight()) * 1 / 3 + i * 60;
            SDL_Rect filled_rect;
            filled_rect.x = X;
            filled_rect.y = Y;
            filled_rect.w = i == 0 ? 77 : 35;
            filled_rect.h = 20;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &filled_rect);
            textMenu[i].render(X, Y, renderer);
        }
        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                    waitUntilKeyPressed();
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_ESCAPE)
                        waitUntilKeyPressed();
                        exit(0);
                    break;

                case SDL_MOUSEBUTTONDOWN: {
                    x = e.motion.x;
                    y = e.motion.y;
                    for (int i = 0; i < MENU_ITEM; ++i) {
                        if(checkInside(x, y, textMenu[i].getRect())) {
                            for (int j = 0; j < MENU_ITEM; ++j) {
                                textMenu[i].free();
                                return i;
                            }
                        }
                    }
                }
                break;
            }
        }
        SDL_RenderPresent(renderer);
    }
}
