#include "screenmanager.h"

screenManager* screenManager::mInstance = NULL;

static void keyDown(SDL_Event curEvent)
{
	screenManager* curInstance = screenManager::Instance();
	if (curInstance->curScreen == SCREENS::SCREEN_MENU &&
		curEvent.key.keysym.sym == SDLK_SPACE)
	{
		curInstance->setupLevel(SCREENS::SCREEN_LEVEL1);
	}
}

screenManager::screenManager() {
	game = gameBase::Instance();
	texture = texture2D::Instance();

	LocalPlayer = new character(game, texture);
	setupLevel(SCREENS::SCREEN_MENU);

	gameBase::Instance()->hookFunction[SDL_KEYDOWN].push_back(&keyDown);
}

screenManager::~screenManager() { }

screenManager* screenManager::Instance()
{
	if (!mInstance)
		mInstance = new screenManager;

	return mInstance;
}

string mapArray[3] =
{
	"mapOne",
	"mapOne",
	"mapTwo"
};

void screenManager::setupLevel(SCREENS screen)
{

	SDL_Event* a = new SDL_Event;
	a->user.data1 = &screen;
	a->type = PREMAPCHANGE;

	game->PushEvent(PREMAPCHANGE, *a);

	// Delete all actors in level
	delete LocalPlayer;
	for (enemy* curEnemy : enemyTable) {

		enemyTable.erase(std::remove(enemyTable.begin(), enemyTable.end(), curEnemy), enemyTable.end());
		delete curEnemy;

	}

	tiles::Instance()->wipeTiles();

	//Setup new actors
	LocalPlayer = new character(game, texture);
	enemyTable.push_back(new enemy(game, texture));

	tiles::Instance()->loadFromFile(mapArray[(int)screen]);

	// Set position of actors
	// TODO

	LocalPlayer->ignoreInput = false;

	switch (screen)
	{

		case SCREENS::SCREEN_MENU:
		{
			LocalPlayer->ignoreInput = true;
			break;
		}

		case SCREENS::SCREEN_LEVEL1: 
		{
			soundManager::Instance()->setVolume(0, 5);
			soundManager::Instance()->playSound(0, -1);
			break;
		}

	}

	a->type = POSTMAPCHANGE;

	game->PushEvent(POSTMAPCHANGE, *a);

	delete a;

	curScreen = screen;
}

void screenManager::render()
{

	switch (curScreen) {

		case SCREENS::SCREEN_MENU:
		{
			renderMenuScreen();
			break;
		}

	}

}

void screenManager::initText(int index, int width, int height)
{
	switch (index) {
		case 0:
		{
			curString[index] = "MARIO BROS.";
			colors[index] = { 211,70,6 };
			Message_rect[index].w = 580;
			Message_rect[index].h = 140;

			Message_rect[index].y = ((SCREEN_HEIGHT / 4) * 0.8);
			Message_rect[index].x = (SCREEN_WIDTH + width) / 15;
			break;
		}

		case 1:
		{
			curString[index] = "Press Space to start game";
			colors[index] = { 255,255,255 };
			Message_rect[index].w = 580;
			Message_rect[index].h = 70;

			Message_rect[index].y = ((SCREEN_HEIGHT / 4) * 1.8);
			Message_rect[index].x = (SCREEN_WIDTH + width) / 9;
			break;
		}
	}
}

void screenManager::renderMenuScreen()
{

	for (int i = 0; i < 2; i++)
	{
		if (!surfaceMessage[i])
		{

			int w, h;

			if (TTF_SizeText(gameBase::Instance()->Sans, curString[i].c_str(), &w, &h))
			{
				return;
			}

			initText(i, w, h);

			screenManager::surfaceMessage[i] = TTF_RenderText_Solid(gameBase::Instance()->Sans, curString[i].c_str(), colors[i]);

			Message[i] = SDL_CreateTextureFromSurface(gameBase::Instance()->gRenderer, surfaceMessage[i]);
		}

		SDL_RenderCopy(gameBase::Instance()->gRenderer, Message[i], NULL, &Message_rect[i]);
	}

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
	LocalPlayer->canMove = true;
	
	for (enemy* enemys : enemyTable) {
		
		if (collisions::Instance()->Box(LocalPlayer, enemys))
		{
			LocalPlayer->canMove = false;
			break;
		}

	}

}