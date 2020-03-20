#include "soundManager.h"

soundManager* soundManager::mInstance = NULL;


soundManager::soundManager()
{

	_waveFileNames =
	{

		"sounds/background_music.wav",
		"sounds/jump_effect.wav"
	};

	int result = Mix_AllocateChannels(_waveFileNames.size());
	if (result < 0)
	{
		printf("Unable to allocate mixing channels: %s\n", SDL_GetError());
		return;
	}

	for (int i = 0; i < _waveFileNames.size(); i++)
	{
		_sample.push_back(Mix_LoadWAV(_waveFileNames[i].c_str()));

		if (_sample[i] == NULL)
			printf("Unable to load wave file: %s\n", _waveFileNames[i].c_str());
	}

}

soundManager::~soundManager()
{

	for (int i = 0; i < _sample.size(); i++)
	{
		Mix_FreeChunk(_sample[i]);
	}

}

soundManager* soundManager::Instance()
{
	if (!mInstance)
		mInstance = new soundManager;

	return mInstance;
}

void soundManager::playSound(int id)
{
	Mix_HaltChannel(id);
	Mix_PlayChannel(id, _sample[id], 0);
}

void soundManager::playSound(int id, int loops)
{
	Mix_HaltChannel(id);
	Mix_PlayChannel(id, _sample[id], loops);
}

void soundManager::setVolume(int id, int volume)
{
	Mix_Volume(id, volume);
}