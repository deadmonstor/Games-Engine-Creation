#pragma once
#include <algorithm>

#include "main.h"
#include "texture.h"
#include "character.h"
#include "enemy.h"
#include "collisions.h"
#include "highscores.h"
#include "powblock.h"

#ifndef _SCREENMANAGER_H
	#define _SCREENMANAGER_H

	class powblock;
	class enemy;
	class character;

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
			SCREENS curScreen = SCREEN_MENU;

			vector< enemy* > enemyTable;
			vector< powblock* > powTable;


		private:

			gameBase* game = NULL;
			texture2D* texture = NULL;

			static screenManager* mInstance;

			
			void updateEnemyCollision();
			void updatePowBlockCollision();


			void renderMenuScreen();
			string curString [2];
			SDL_Surface* surfaceMessage [2];
			SDL_Texture* Message [2];
			SDL_Rect Message_rect [2];
			SDL_Color colors[2];
	};

#endif