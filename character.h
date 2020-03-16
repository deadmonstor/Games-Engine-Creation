#pragma once

#ifndef _CHARACTER_H

	#define _CHARACTER_H
	#include "main.h"
	#include "texture.h"

	#define JUMP_FORCE 200
	#define JUMP_FORCE_DECREMENT 400

	class character {

		public:
			character(gameBase* gameBases, texture2D* textures);
			~character();

			SDL_Rect getPosition();
			SDL_Rect getRenderBox();

			void renderCharacter(SDL_Event event);
			void keyDown(SDL_Event event);
			void keyUp(SDL_Event event);
			void Update(SDL_Event event);
			void Jump();
			void addGravity(SDL_Event event, float deltaTime);

			bool canMove;

		private:

			gameBase* game;
			SDL_Texture* curTexture;
			SDL_Rect imgPartRect;
			SDL_Rect DestR;

			int jumpForce;
			bool jumping;
			bool canJump;

			FACING curFacing;
			map<SDL_Keycode, bool> curDown;

	};

#endif