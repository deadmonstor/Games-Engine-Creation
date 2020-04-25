#pragma once

#ifndef _GAMEBASE_H
	#include "main.h"
#endif

#ifndef _TEXTURE_H
	#include "texture.h"
#endif

#ifndef _COLLISIONS_H
	#include "collisions.h"
#endif

#ifndef _TILES_H
	#include "tiles.h"
#endif

#ifndef _ENEMY_H
	#define _ENEMY_H
	class enemy {

	private:
		SDL_Texture* curTexture;
		SDL_Rect imgPartRects;
		SDL_Rect DestRs;
		FACING curFacing = FACING::RIGHT;
		bool canMove;
		bool isDistrupted = false;
		bool moveDistrupted = false;

		Uint32 lastChange = 0;

	public:
		enemy(int x, int y);
		~enemy();

		SDL_Rect getPosition();
		SDL_Rect getRenderBox();
		void render(SDL_Event _);
		void update(SDL_Event _);
		void addGravity(SDL_Event _, float deltaTime);
		pair<int, int> getTilePos();
		pair<int, int> getTilePos(int x, int y);
		bool getCanMove();

		void setDistrupted(bool);
		bool getDistrupted();
		void setMoveDistrupted(bool);
		bool getMoveDistrupted();
		void setDirection(FACING direction);
		Uint32 getLastChange();

	};

#endif // !ENEMY_H
