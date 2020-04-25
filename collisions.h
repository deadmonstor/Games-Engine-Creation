#pragma once

#ifndef _COLLISIONS_H
	#define _COLLISIONS_H

	#include "main.h"
	#include "character.h"
	#include "enemy.h"
	#include "powblock.h"

	class character;
	class enemy;
	class tile;
	class powblock;

	class collisions
	{
		public:
			~collisions();

			static collisions* Instance();
			bool Circle(character* chracter1, character* character2);
			bool Circle(character* chracter1, enemy* character2);
			bool Circle(enemy* chracter1, tile* character2);
			bool Circle(enemy* chracter1, enemy* character2);
			bool Box(character* chracter1, character* character2);
			bool Box(character* chracter1, enemy* character2);
			bool Box(character* chracter1, tile* tile);
			bool Box(enemy* chracter1, tile* tile);
			bool Box(character* character1, powblock* curPowBlock);

		private:
			static collisions* mInstance;
			collisions();

	};

#endif
