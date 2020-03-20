#pragma once
#include "main.h"
#include "texture.h"

#define SIZE 32

class tile
{
	public:
		int x, y;

		tile(int _x, int _y);
		void render();

	private:
		map<int, map<int, bool>> collisionCache;
		SDL_Texture* curTexture;
		SDL_Rect imgPartRect;
		SDL_Rect DestR;
};

class tiles
{
	public:

		tiles();
		~tiles();

		void render();
		static tiles* Instance();

	private:
		map<int, map<int, tile*>> tileMap;
		static tiles* mInstance;
};

