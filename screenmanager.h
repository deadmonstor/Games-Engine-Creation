#pragma once
#include "main.h"
#include "character.h"
#include "texture.h"
#include "enemy.h"
#include "collisions.h"

#ifndef _SCREENMANAGER_H
	#define _SCREENMANAGER_H

	class screenManager {

		public:
			screenManager(gameBase* gameBases, texture2D* textures);
			~screenManager();

			void setupLevel(SCREENS screen);
			void update();

		private:

			gameBase* game = NULL;
			character* LocalPlayer = NULL;
			enemy* enemys = NULL;
			texture2D* texture = NULL;

			SCREENS curScreen = SCRENE_INTRO;

			
			void updateScreenOne();
	};

#endif