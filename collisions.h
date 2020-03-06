#pragma once
#include "main.h"
#include "character.h"

class collisions
{
	public:
		~collisions();

		static collisions* Instance();
		bool Circle(character* chracter1, character* character2);
		bool Box(SDL_Rect rect1, SDL_Rect rect2);

	private:
		static collisions* mInstance;
		collisions();

};

