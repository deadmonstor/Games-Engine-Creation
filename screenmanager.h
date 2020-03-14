#pragma once
#include "main.h"
#include "character.h"
#include "texture.h"
#include "enemy.h"
#include "collisions.h"
#include <algorithm>

#ifndef _SCREENMANAGER_H
	#define _SCREENMANAGER_H

	class screenManager {

		public:
			screenManager(gameBase* gameBases, texture2D* textures);
			~screenManager();

			void setupLevel(SCREENS screen);
			void update();

			character* LocalPlayer = NULL;

		private:

			gameBase* game = NULL;
			vector< enemy* > enemyTable;
			texture2D* texture = NULL;

			SCREENS curScreen = SCRENE_INTRO;

			
			void updateScreenOne();
	};

#endif