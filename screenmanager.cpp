#include "screenmanager.h"

screenManager::screenManager(gameBase* gameBases, texture2D* textures) {
	game = gameBases;
	texture = textures;

	LocalPlayer = new character(game, texture);
	setupLevel(SCREENS::SCREEN_LEVEL1); // Change to intro on start
}

screenManager::~screenManager() { }

void screenManager::setupLevel(SCREENS screen)
{

	SDL_Event* a = new SDL_Event;
	a->user.data1 = &screen;
	a->type = PREMAPCHANGE;

	game->PushEvent(PREMAPCHANGE, *a);

	// Delete all actors in level
	delete LocalPlayer;

	//Setup new actors
	LocalPlayer = new character(game, texture);
	enemys = new enemy(game, texture);

	// Set position of actors
	// TODO

	a->type = POSTMAPCHANGE;

	game->PushEvent(POSTMAPCHANGE, *a);

	delete a;

	curScreen = screen;
}

void screenManager::update()
{
	switch (curScreen) {

		case SCREENS::SCREEN_LEVEL1:
		{
			updateScreenOne();
			break;
		}

	}

}

void screenManager::updateScreenOne()
{
	if (collisions::Instance()->Box(LocalPlayer, enemys)) 
	{
		cout << "test" << endl;
	}
}