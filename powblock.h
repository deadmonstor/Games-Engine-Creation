#pragma once

#ifndef _SCREENMANAGER_H
	#include "screenmanager.h"
#endif


class powblock
{

	public:

		powblock(int x, int y);
		~powblock();
		void render();
		void onHit();
		void onDestroy();
		void enemyDisrupt();
		pair<int, int> getTilePos();
		pair<int, int> getTilePos(int x, int y);
		SDL_Rect getRenderBox();
		SDL_Rect getPosition();

	private: 
		int mCurHit = 0;
		Uint32 lastUsed = 0;
		SDL_Texture* mTexture;
		SDL_Rect imgRect;
		SDL_Rect positionRect;
};

