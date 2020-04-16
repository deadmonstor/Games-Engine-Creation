#pragma once
#include "main.h"

#ifndef _HIGHSCORES_H
	#define _HIGHSCORES_H

	#include <string>
	#include "screenmanager.h"

	class highscores
	{
		public:
			highscores();

			void render();
			static highscores* Instance();
			int curScore = 0;

			struct Score {
				string name = "N/A";
				int score = 0;
			};

			void readFile();
			void writeFile();
			void addScore(string name, int point);
			void displayScores();
			void sortScores();

			#define MAX_SCORES 3
			Score scores[MAX_SCORES];

		private:
		

			static highscores* mInstance;
			string curScoreString;
			SDL_Surface* surfaceMessage;
			SDL_Texture* Message;
			SDL_Rect Message_rect;

	};



#endif