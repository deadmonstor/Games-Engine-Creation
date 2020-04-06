#pragma once

#ifndef _COLLISIONS_H
	#define _COLLISIONS_H

	#include "main.h"
	#include "character.h"
	#include "enemy.h"

	class character;
	class enemy;
	class tile;

	class collisions
	{
		public:
			~collisions();

			static collisions* Instance();
			bool Circle(character* chracter1, character* character2);
			bool Circle(character* chracter1, enemy* character2);
			bool Box(character* chracter1, character* character2);
			bool Box(character* chracter1, enemy* character2);
			bool Box(character* chracter1, tile* tile);

		private:
			static collisions* mInstance;
			collisions();

	};

#endif
