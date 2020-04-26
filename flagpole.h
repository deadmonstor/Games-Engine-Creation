#pragma once
#include "main.h"
#include "texture.h"

class flagpole
{
	public:
		flagpole(int x, int y);
		void render();
		SDL_Rect getRenderBox();
		void updateTexture(int y);

	private: 
		SDL_Texture* curTexture;
		SDL_Rect imgPartRect;
		SDL_Rect DestR;

		vector<int> flagPositions = { 116, 116, 149, 181, 215, 249, 249 };
};

