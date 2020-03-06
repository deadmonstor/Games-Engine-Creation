#include "screenmanager.h"

screenManager::screenManager(gameBase* gameBases, texture2D* textures) {
	game = gameBases;
	texture = textures;

	LocalPlayer = new character(game, texture);
	setupLevel(SCREENS::SCRENE_INTRO);
}

screenManager::~screenManager() { }

void screenManager::setupLevel(SCREENS screen)
{

	SDL_Event* a = new SDL_Event;
	a->user.data1 = &screen;
	a->type = PREMAPCHANGE;

	game->PushEvent(PREMAPCHANGE);

	// Delete all actors in level
	delete LocalPlayer;

	//Setup new actors
	LocalPlayer = new character(game, texture);
	enemys = new enemy(game, texture);

	// Set position of actors
	// TODO

	a->type = POSTMAPCHANGE;

	game->PushEvent(POSTMAPCHANGE);

	delete a;
}