#pragma once
#include "main.h"
#include "character.h"
#include "texture.h"
#include "enemy.h"

#ifndef _SCREENMANAGER_H
	#define _SCREENMANAGER_H

	class screenManager {

		public:
			screenManager(gameBase* gameBases, texture2D* textures);
			~screenManager();

			void setupLevel(SCREENS screen);

		private:

			gameBase* game = NULL;
			character* LocalPlayer = NULL;
			enemy* enemys = NULL;
			texture2D* texture = NULL;

			SCREENS curScreen = SCRENE_INTRO;
	};

#endif