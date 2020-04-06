#pragma once

#ifndef _GAMEBASE_H
	#include "main.h"
#endif

#ifndef _TEXTURE_H
	#include "texture.h"
#endif

#ifndef _ENEMY_H
	#define _ENEMY_H
	class enemy {

	public:
		enemy(gameBase* gameBases, texture2D* textures);
		~enemy();

		SDL_Rect getPosition();
		SDL_Rect getRenderBox();
	};

#endif // !ENEMY_H
