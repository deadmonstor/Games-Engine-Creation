#pragma once

#ifndef _GAMEBASE_H
	#define _GAMEBASE_H

	using namespace std;
	
	#define SCREEN_WIDTH 1280
	#define SCREEN_HEIGHT 640

	#define RENDERUPDATE 1
	#define PRETICK 2
	#define POSTTICK 3
	#define PREMAPCHANGE 4
	#define POSTMAPCHANGE 5

	enum SCREENS
	{
		SCRENE_INTRO = 0,
		SCREEN_MENU,
		SCREEN_LEVEL1,
		SCREEN_LEVEL2,
		SCREEN_GAMEOVER,
		SCREEN_HIGHSCORES
	};

	enum FACING
	{
		LEFT = 0,
		RIGHT
	};

	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_mixer.h>
	#include <SDL_ttf.h>

	#include <iostream>
	#include <map>
	#include <vector>
	#include <time.h>

	class gameBase {

		public:

			gameBase();

			SDL_Window* gWindow = NULL;
			SDL_Renderer* gRenderer = NULL;
			SDL_Texture* gTexture = NULL;

			static gameBase* Instance();

			bool SDLInit();
			void SDLClose();

			bool Update(SDL_Event event, float deltaTime);
			void Render();

			void PushEvent(int id);
			void PushEvent(int id, SDL_Event event);

			map<int, vector<void (*)(SDL_Event event)>> hookFunction;
			map<int, vector<void (*)(SDL_Event event, void * this_pointer)>> hookFunctionCharacter;

			bool shouldQuit = false;

		private:
			TTF_Font* Sans;
			static gameBase* mInstance;

	};

	class Vector2D {
	public:
		int x, y;

		Vector2D() :x(0), y(0) {}
		Vector2D(int x, int y) : x(x), y(y) {}
	};

#endif