#include "texture.h"
#include "tiles.h"

#ifndef _SCREENMANAGER_H
	#include "screenmanager.h"
#endif

#include "highscores.h"
#include "main.h"

int main(int argc, char* args[])
{
	gameBase* game = gameBase::Instance();

	/*
		INIT MODULES
	*/

	texture2D::Instance(game);

	float tickCount = 0;
	float deltaTime = 0;

	game->SDLInit();

	screenManager::Instance();
	screenManager::Instance()->setupLevel(SCREENS::SCREEN_MENU);

	while (!game->shouldQuit)
	{

		deltaTime = (SDL_GetTicks() - tickCount) / 1000.0f;

		if (deltaTime > 0.5f)
		{
			deltaTime = 0.5f;
		}

		tickCount = SDL_GetTicks();

		game->Render();

		SDL_Event e;
		SDL_PollEvent(&e);


		game->shouldQuit = game->Update(e, deltaTime);

	}

	return 0;
}

void gameBase::PushEvent(int id)
{
	int maxSize = hookFunction[id].size();

	for (int i = 0; i < maxSize; i++)
	{
		vector<void (*)(SDL_Event event)> vectorArray = hookFunction[id];

		SDL_Event* a = new SDL_Event;
		a->type = id;

		vectorArray.at(i)(*a);

		delete a;
	}

	maxSize = hookFunctionCharacter[id].size();

	for (int i = 0; i < maxSize; i++)
	{
		if (screenManager::Instance()->LocalPlayer == nullptr) continue;
		vector<void (*)(SDL_Event event, void* this_pointer)> vectorArray = hookFunctionCharacter[id];

		SDL_Event* a = new SDL_Event;
		a->type = id;

		vectorArray.at(i)(*a, screenManager::Instance()->LocalPlayer);

		delete a;
	}

	maxSize = hookFunctionEnemies[id].size();

	for (int i = 0; i < maxSize; i++)
	{
		vector<void (*)(SDL_Event event, void* this_pointer)> vectorArray = hookFunctionEnemies[id];

		SDL_Event* a = new SDL_Event;
		a->type = id;

		for (enemy* enemys : screenManager::Instance()->enemyTable)
		{
			vectorArray.at(i)(*a, enemys);
		}

		delete a;
	}

}

void gameBase::PushEvent(int id, SDL_Event event)
{
	int maxSize = hookFunction[id].size();

	for (int i = 0; i < maxSize; i++)
	{
		vector<void (*)(SDL_Event event)> vectorArray = hookFunction[id];

		vectorArray.at(i)(event);
	}

	maxSize = hookFunctionCharacter[id].size();

	for (int i = 0; i < maxSize; i++)
	{
		if (screenManager::Instance()->LocalPlayer == nullptr) continue;
		vector<void (*)(SDL_Event event, void* this_pointer)> vectorArray = hookFunctionCharacter[id];
		vectorArray.at(i)(event, screenManager::Instance()->LocalPlayer);
	}


	maxSize = hookFunctionEnemies[id].size();

	for (int i = 0; i < maxSize; i++)
	{
		vector<void (*)(SDL_Event event, void* this_pointer)> vectorArray = hookFunctionEnemies[id];

		for (enemy* enemys : screenManager::Instance()->enemyTable)
		{
			vectorArray.at(i)(event, enemys);
		}
	}
}


bool gameBase::Update(SDL_Event event, float deltaTime)
{

	SDL_Event* a = new SDL_Event;
	a->user.data1 = &deltaTime;
	a->type = PRETICK;

	gameBase::PushEvent(PRETICK, *a);

	screenManager::Instance()->update();

	gameBase::PushEvent(event.type, event);

	a->type = POSTTICK;

	gameBase::PushEvent(POSTTICK, *a);

	switch (event.type)
	{
		case SDL_QUIT:
			return true;
			break;
	}

	return false;
}

const int timeStart = time(NULL);

void gameBase::Render()
{

	SDL_SetRenderDrawColor(gRenderer, (Uint8)87, (Uint8)120, (Uint8)255, (Uint8)255);
	SDL_RenderClear(gRenderer);

	tiles::Instance()->render();
	highscores::Instance()->render();
	screenManager::Instance()->render();
	gameBase::PushEvent(RENDERUPDATE);

	SDL_RenderPresent(gRenderer);

}

/*

	== WINDOW FUNCTIONS == 

*/
bool gameBase::SDLInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

		printf("error initializing SDL: %s\n", SDL_GetError());
		return false;

	}
	else
	{

		gWindow = SDL_CreateWindow("GAME",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);


		if (gWindow == NULL)
		{
			printf("Error creating window: %s\n", SDL_GetError());
			return false;
		}

	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (gRenderer != NULL)
	{

		int imageFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imageFlags) & imageFlags))
		{
			printf("Error creating texture: %s\n", IMG_GetError());
			return false;
		}

	}
	else
	{

		printf("Renderer could not be initialised: %s\n", SDL_GetError());
		return false;

	}

	gTexture = texture2D::Instance()->LoadTextureFromFile("Images/11.png");

	if (gTexture == NULL) 
	{
		return false;
	}

	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		return false;
	}

	if (Sans == NULL)
		Sans = TTF_OpenFont("font/supermariobros.ttf", 100);

	int result = Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048);
	if (result < 0)
	{
		printf("Unable to open audio: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void gameBase::SDLClose()
{

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	Mix_CloseAudio();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

}

gameBase* gameBase::mInstance = gameBase::mInstance == nullptr ? NULL : gameBase::mInstance;

gameBase::gameBase()
{

}

gameBase* gameBase::Instance()
{
	if (!mInstance)
		mInstance = new gameBase;

	return mInstance;
}