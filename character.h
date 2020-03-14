#pragma once

#ifndef _CHARACTER_H

	#define _CHARACTER_H
	#include "main.h"
	#include "texture.h"

	class character {

		public:
			character(gameBase* gameBases, texture2D* textures);
			~character();

			SDL_Rect getPosition();
			SDL_Rect getRenderBox();

			void renderCharacter(SDL_Event event);
			void keyDown(SDL_Event event);

		private:
			gameBase* game;
			SDL_Texture* curTexture;
			SDL_Rect imgPartRect;
			SDL_Rect DestR;
			bool canMove;
	};

#endif