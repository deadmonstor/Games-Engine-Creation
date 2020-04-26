#pragma once
#include "main.h"
#include "texture.h"

class flagpole
{
	public:
		flagpole(int x, int y);
		void render();
		SDL_Rect getRenderBox();


	private: 
		SDL_Texture* curTexture;
		SDL_Rect imgPartRect;
		SDL_Rect DestR;
};

