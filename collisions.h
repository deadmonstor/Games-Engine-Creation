#pragma once
#include "main.h"
#include "character.h"
#include "enemy.h"

class collisions
{
	public:
		~collisions();

		static collisions* Instance();
		bool Circle(character* chracter1, character* character2);
		bool Circle(character* chracter1, enemy* character2);
		bool Box(character* chracter1, character* character2);
		bool Box(character* chracter1, enemy* character2);

	private:
		static collisions* mInstance;
		collisions();

};

