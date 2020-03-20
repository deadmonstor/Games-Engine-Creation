#pragma once

#include "main.h"

class soundManager
{
	public:
		~soundManager();

		static soundManager* Instance();
		
		void playSound(int id);
		void playSound(int id, int loops);
		void setVolume(int id, int volume);

	private:
		static soundManager* mInstance;
		soundManager();
		vector<string> _waveFileNames;
		vector<Mix_Chunk*> _sample;
};

