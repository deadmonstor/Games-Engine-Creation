#pragma once

#ifndef _GAMEBASE_H
	#define _GAMEBASE_H

	using namespace std;
	
	#define SCREEN_WIDTH 320 * 4
	#define SCREEN_HEIGHT 240 * 2.5

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

			SDL_Window* gWindow = NULL;
			SDL_Renderer* gRenderer = NULL;
			SDL_Texture* gTexture = NULL; 

			bool SDLInit();
			void SDLClose();

			bool Update(SDL_Event event);
			void Render();

			void PushEvent(int id);
			void PushEvent(int id, SDL_Event event);

			map<int, vector<void (*)(SDL_Event event)>> hookFunction;

			bool shouldQuit = false;

	};

	class Vector2D {
	public:
		int x, y;

		Vector2D() :x(0), y(0) {}
		Vector2D(int x, int y) : x(x), y(y) {}
	};

#endif