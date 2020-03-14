#pragma once
#include "main.h"
#include "texture.h"

class enemy {

	public:
		enemy(gameBase* gameBases, texture2D* textures);
		~enemy();

		SDL_Rect getPosition();
		SDL_Rect getRenderBox();
};