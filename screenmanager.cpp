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
	for (enemy* curEnemy : enemyTable) 
	{
		enemyTable.erase(std::remove(enemyTable.begin(), enemyTable.end(), curEnemy), enemyTable.end());
		delete curEnemy;
	}

	enemyTable.clear();
	enemyTable.resize(0);

	for (powblock* curPowBlock : powTable)
	{
		powTable.erase(std::remove(powTable.begin(), powTable.end(), curPowBlock), powTable.end());
		delete curPowBlock;
	}

	powTable.clear();
	powTable.resize(0);

	for (flagpole* curFlagPole : flagPoleTable)
	{
		flagPoleTable.erase(std::remove(flagPoleTable.begin(), flagPoleTable.end(), curFlagPole), flagPoleTable.end());
		delete curFlagPole;
	}

	flagPoleTable.clear();
	flagPoleTable.resize(0);


	tiles::Instance()->wipeTiles();

	//Setup new actors
	LocalPlayer = new character(game, texture);
	enemyTable.push_back(new enemy(4 * 32, 1 * 32));
	enemyTable.push_back(new enemy(3 * 32, 1 * 32));
	powTable.push_back(new powblock(12 * 32, 11 * 32));
	flagPoleTable.push_back(new flagpole(35 * 32, 8 * 32));

	tiles::Instance()->loadFromFile(mapArray[(int)screen]);

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
	for (powblock* powBlocks : powTable)
	{
		if (powBlocks == nullptr) continue;
		powBlocks->render();
	}

	for (flagpole* flagPole : flagPoleTable)
	{
		if (flagPole == nullptr) continue;
		flagPole->render();
	}

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
	updateEnemyCollision();
	updatePowBlockCollision();
	updateFlagPoleCollision();
}

void screenManager::updateEnemyCollision()
{
	int curPosXLocalPlayer = get<0>(LocalPlayer->getTilePos()), curPosYLocalPlayer = get<1>(LocalPlayer->getTilePos());
	SDL_Rect curPosition = LocalPlayer->getPosition();
	for (enemy* enemys : enemyTable) 
	{
		
		bool canMoveProperly = (enemys->getCanMove() && (!enemys->getDistrupted() && !enemys->getMoveDistrupted() 
								|| enemys->getDistrupted() && enemys->getMoveDistrupted()));

		int curPosXEnemy = get<0>(enemys->getTilePos()), curPosYEnemy = get<1>(enemys->getTilePos());
		SDL_Rect curEnemyPosition = enemys->getPosition();


		if (canMoveProperly && (enemys->getLastChange() < SDL_GetTicks() - 1000) && collisions::Instance()->Box(LocalPlayer, enemys) && !(curPosYLocalPlayer < curPosYEnemy))
		{
			// Jumped into enemy while not distrupted == DEATH
			LocalPlayer->hasDied = true;
			break;
		}
		else if (collisions::Instance()->Circle(LocalPlayer, enemys) && !LocalPlayer->hasDied && (curPosYLocalPlayer < curPosYEnemy))
		{
			// Jumped ontop of enemy == Destroy

			std::vector<enemy*>::iterator position = std::find(enemyTable.begin(), enemyTable.end(), enemys);
			if (position != enemyTable.end())
				enemyTable.erase(position);

			highscores::Instance()->curScore++;

			delete enemys;
		}
		else if ((enemys->getDistrupted() && !enemys->getMoveDistrupted()) && collisions::Instance()->Circle(LocalPlayer, enemys) && !LocalPlayer->hasDied)
		{
			// Jumped into enemy while distrupted == spin object away
			enemys->setMoveDistrupted(true);
			enemys->setDirection((curPosition.x > curEnemyPosition.x ? FACING::LEFT : FACING::RIGHT));
		}
		else if ((enemys->getDistrupted() && enemys->getMoveDistrupted()))
		{
			// Scan through all "enemies" to see if it hits the other one
			for (enemy* secondEnemy : enemyTable)
			{
				// If it hits the enemy and isn't the first enemy. ABORT DEATH
				if (secondEnemy != enemys && collisions::Instance()->Circle(enemys, secondEnemy))
				{
					std::vector<enemy*>::iterator position = std::find(enemyTable.begin(), enemyTable.end(), secondEnemy);
					if (position != enemyTable.end())
						enemyTable.erase(position);

					highscores::Instance()->curScore++;

					delete secondEnemy;
				}

			}
		}

	}
}

void screenManager::updatePowBlockCollision()
{

	SDL_Rect curPosition = LocalPlayer->getPosition();
	for (powblock* curPowBlock : powTable)
	{
		SDL_Rect curPowBlockPosition = curPowBlock->getPosition();

		if (collisions::Instance()->Box(LocalPlayer, curPowBlock) && !LocalPlayer->hasDied && curPosition.y > curPowBlockPosition.y)
		{
			curPowBlock->onHit();
			LocalPlayer->cancelJump();
		}

	}

}

void screenManager::updateFlagPoleCollision()
{

	for (flagpole* curFlagPole : flagPoleTable)
	{

		bool test = collisions::Instance()->Box(LocalPlayer, curFlagPole) || LocalPlayer->stopGravity;
		if (test && !LocalPlayer->hasDied)
		{
			LocalPlayer->cancelJump();
			LocalPlayer->clearInput();
			LocalPlayer->ignoreInput = true;
			LocalPlayer->stopGravity = true;

			SDL_Rect dest = LocalPlayer->getPosition();
			dest.x = curFlagPole->getRenderBox().x;
			dest.y++;

			LocalPlayer->setPosition(dest);

			if (dest.y > (curFlagPole->getRenderBox().y + (32 * 7 / 2)))
			{
				setupLevel(SCREENS::SCREEN_LEVEL2);
			}

			
		}

	}

}