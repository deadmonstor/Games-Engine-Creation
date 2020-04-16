#pragma once
#include <algorithm>

#include "main.h"
#include "texture.h"
#include "character.h"
#include "enemy.h"
#include "collisions.h"

#ifndef _SCREENMANAGER_H
	#define _SCREENMANAGER_H

	class screenManager {

		public:
			screenManager();
			~screenManager();

			void setupLevel(SCREENS screen);
			void update();
			void render();
			void initText(int index, int width, int height);
			static screenManager* Instance();

			character* LocalPlayer = NULL;
			SCREENS curScreen = SCRENE_INTRO;


		private:

			gameBase* game = NULL;
			vector< enemy* > enemyTable;
			texture2D* texture = NULL;

			static screenManager* mInstance;

			
			void updateScreenOne();


			void renderMenuScreen();
			string curString [2];
			SDL_Surface* surfaceMessage [2];
			SDL_Texture* Message [2];
			SDL_Rect Message_rect [2];
			SDL_Color colors[2];
	};

#endif