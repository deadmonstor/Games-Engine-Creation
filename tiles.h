#pragma once

#include "main.h"
#include "texture.h"

#ifndef _TILES_H
	#define _TILES_H
	#define SIZE 32

	class tile
	{
		public:
			int x, y;

			tile(int _x, int _y);
			void render();
			SDL_Rect getCollisionBox();

		private:
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
			map<int, map<int, tile*>>* getTileMap();

		private:
			map<int, map<int, tile*>> tileMap;
			static tiles* mInstance;
	};

#endif // !_TILES_H