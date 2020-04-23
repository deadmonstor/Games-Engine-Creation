#pragma once

#include "main.h"

#ifndef _TILES_H
	#define _TILES_H
	#define SIZE 32

	#include "texture.h"

	class tile
	{
		public:
			int x, y;

			tile(int _x, int _y);
			tile(int _x, int _y, string *textureName);
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
			void loadFromFile(string fileName);
			void wipeTiles();
			static tiles* Instance();
			map<int, map<int, tile*>>* getTileMap();

		private:
			map<int, map<int, tile*>> tileMap;
			static tiles* mInstance;
	};

#endif // !_TILES_H