#ifndef SDL_text__H_
#define SDL_text__H_

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "SDL_text.h"

using namespace std;

class LTexture {
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path, SDL_Renderer* renderer);
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* font);

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

		SDL_Rect getRect();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
		int X;
		int Y;
};

#endif // SDL_text
