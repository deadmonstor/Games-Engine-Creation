#pragma once
#include "main.h"
#include "texture.h"

class character {

	public:
		character(gameBase* gameBases, texture2D* textures);
		~character();

		SDL_Rect getPosition();
		SDL_Rect getRenderBox();
};