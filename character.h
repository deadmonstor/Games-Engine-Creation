#pragma once
#include "main.h"
#include "texture.h"

class character {

	public:
		character(gameBase* gameBases, texture2D* textures);
		~character();


	private:
		bool isColliding(SDL_Rect enemy);
};