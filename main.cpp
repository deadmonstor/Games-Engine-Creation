#include "texture.h"
#include "tiles.h"
#include "screenmanager.h"
#include "highscores.h"
#include "main.h"


texture2D* texture;
screenManager* gameScreenManager;

int main(int argc, char* args[])
{
	gameBase* game = gameBase::Instance();

	/*
		INIT MODULES
	*/

	texture = texture2D::Instance(game);

	float tickCount = 0;
	float deltaTime = 0;

	game->SDLInit();

	gameScreenManager = new screenManager(game, texture);

	while (!game->shouldQuit)
	{
		deltaTime = (SDL_GetTicks() - tickCount) / 1000.0f;

		if (deltaTime > 0.05f)
		{
			deltaTime = 0.05f;
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
		vector<void (*)(SDL_Event event, void* this_pointer)> vectorArray = hookFunctionCharacter[id];

		SDL_Event* a = new SDL_Event;
		a->type = id;

		vectorArray.at(i)(*a, gameScreenManager->LocalPlayer);

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
		vector<void (*)(SDL_Event event, void* this_pointer)> vectorArray = hookFunctionCharacter[id];
		vectorArray.at(i)(event, gameScreenManager->LocalPlayer);
	}
}

bool gameBase::Update(SDL_Event event, float deltaTime)
{

	SDL_Event* a = new SDL_Event;
	a->user.data1 = &deltaTime;
	a->type = PRETICK;

	gameBase::PushEvent(PRETICK, *a);

	gameBase::PushEvent(event.type, event);

	gameScreenManager->update();

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

	SDL_SetRenderDrawColor(gRenderer, (Uint8)255, (Uint8)255, (Uint8)255, (Uint8)255); // TODO: Create a colour class so I don't have to do this
	SDL_RenderClear(gRenderer);


	SDL_Rect renderLocation = { timeStart - time(NULL) , 0, 7168, SCREEN_HEIGHT + 600 }; // TODO: Create a Vector() and Location construction for this I don't want to be creating array's everywhere

	//SDL_RenderCopyEx(gRenderer, gTexture, NULL, &renderLocation, 0, NULL, SDL_FLIP_NONE);

	tiles::Instance()->render();
	highscores::Instance()->render();
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

	gTexture = texture->LoadTextureFromFile("Images/11.png");

	if (gTexture == NULL) 
	{
		return false;
	}

	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		return false;
	}

	if (Sans == NULL)
		Sans = TTF_OpenFont("OpenSans-Regular.ttf", 24);

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

gameBase* gameBase::mInstance = NULL;

gameBase::gameBase()
{

}

gameBase* gameBase::Instance()
{
	if (!mInstance)
		mInstance = new gameBase;

	return mInstance;
}