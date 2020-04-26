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

	if (curInstance->curScreen == SCREENS::SCREEN_LEVELCHANGE &&
		curEvent.key.keysym.sym == SDLK_SPACE)
	{
		curInstance->setupLevel(SCREENS::SCREEN_LEVEL2);
	}

	if (curInstance->curScreen == SCREENS::SCREEN_GAMEOVER &&
		curEvent.key.keysym.sym == SDLK_SPACE)
	{
		curInstance->setupLevel(SCREENS::SCREEN_LEVEL1);
		highscores::Instance()->curScore = 0;
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

string mapArray[7] =
{
	"mapOne",
	"mapOne",
	"mapTwo",
	"",
	"",
	"",
	""
};

void screenManager::setupLevel(SCREENS screen)
{

	SDL_Event* a = new SDL_Event;
	a->user.data1 = &screen;
	a->type = PREMAPCHANGE;

	game->PushEvent(PREMAPCHANGE, *a);

	// Delete all actors in level
	delete LocalPlayer;
	LocalPlayer = nullptr;
	for (enemy* curEnemy : enemyTable) 
	{
		enemyTable.erase(std::remove(enemyTable.begin(), enemyTable.end(), curEnemy), enemyTable.end());
		delete curEnemy;
	}

	enemyTable.clear();

	for (powblock* curPowBlock : powTable)
	{
		powTable.erase(std::remove(powTable.begin(), powTable.end(), curPowBlock), powTable.end());
		delete curPowBlock;
	}

	powTable.clear();

	for (flagpole* curFlagPole : flagPoleTable)
	{
		flagPoleTable.erase(std::remove(flagPoleTable.begin(), flagPoleTable.end(), curFlagPole), flagPoleTable.end());
		delete curFlagPole;
	}

	flagPoleTable.clear();

	memset(surfaceMessage, 0, sizeof surfaceMessage);
	memset(Message, 0, sizeof Message);
	memset(Message_rect, 0, sizeof Message_rect);
	memset(colors, 0, sizeof colors);

	tiles::Instance()->wipeTiles();

	//Setup new actors
	if (screen != SCREEN_LEVELCHANGE && screen != SCREEN_GAMEOVER)
	{
		LocalPlayer = new character(game, texture);
		enemyTable.push_back(new enemy(4 * 32, 1 * 32));
		enemyTable.push_back(new enemy(3 * 32, 1 * 32));
		powTable.push_back(new powblock(12 * 32, 11 * 32));
		flagPoleTable.push_back(new flagpole(32 * 32, 8 * 32));
	}
	else if(screen != SCREEN_GAMEOVER)
	{
		highscores::Instance()->addScore(to_string(curScreen), highscores::Instance()->curScore);
		highscores::Instance()->rememberScore = highscores::Instance()->curScore;
		highscores::Instance()->rememberLevel = curScreen;
		highscores::Instance()->curScore = 0;
	}

	tiles::Instance()->loadFromFile(mapArray[(int)screen]);

	if (LocalPlayer != nullptr)
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

		case SCREENS::SCREEN_LEVELCHANGE:
		{
			renderLevelChangeScreen();
			break;
		}

		case SCREENS::SCREEN_GAMEOVER:
		{
			renderDeathScreen();
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

			Message_rect[index].y = ((SCREEN_HEIGHT / 4) * 0.6);
			Message_rect[index].x = (SCREEN_WIDTH + width) / 55;
			break;
		}

		case 1:
		{
			curString[index] = "Press Space to start game";
			colors[index] = { 255,255,255 };
			Message_rect[index].w = 580;
			Message_rect[index].h = 70;

			Message_rect[index].y = ((SCREEN_HEIGHT / 4) * 1.6);
			Message_rect[index].x = (SCREEN_WIDTH + width) / 16;
			break;
		}

		case 2:
		{
			curString[index] = "Press Space to continue game";
			colors[index] = { 255,255,255 };
			Message_rect[index].w = 580;
			Message_rect[index].h = 70;

			Message_rect[index].y = ((SCREEN_HEIGHT / 4) * 2.5);
			Message_rect[index].x = (SCREEN_WIDTH / 4);
			break;
		}

		case 3:
		{
			curString[index] = "Score: " + to_string(highscores::Instance()->rememberScore);
			colors[index] = { 255,255,255 };
			Message_rect[index].w = 280;
			Message_rect[index].h = 70;

			Message_rect[index].y = ((SCREEN_HEIGHT / 4) * 0.5);
			Message_rect[index].x = (SCREEN_WIDTH / 2.8);
			break;
		}

		case 4:
		{
			int highScore = highscores::Instance()->getHighestScoreOfInt(highscores::Instance()->rememberLevel);
			int myScore = highscores::Instance()->rememberScore;

			if (highScore <= myScore)
			{
				curString[index] = "HighScore: " + to_string(myScore);
				colors[index] = { 255,0,0 };
			}
			else 
			{
				curString[index] = "HighScore: " + to_string(highScore);
				colors[index] = { 255,255,255 };
			}

			Message_rect[index].w = 400;
			Message_rect[index].h = 70;

			Message_rect[index].y = ((SCREEN_HEIGHT / 4) * 1.5);
			Message_rect[index].x = (SCREEN_WIDTH / 3);
			break;
		}

		case 5:
		{
			int highScore = highscores::Instance()->getHighestScoreOfInt(highscores::Instance()->rememberLevel);
			int myScore = highscores::Instance()->rememberScore;

			curString[index] = "You have died.";

			Message_rect[index].w = 400;
			Message_rect[index].h = 70;

			Message_rect[index].y = ((SCREEN_HEIGHT / 4) * 1);
			Message_rect[index].x = (SCREEN_WIDTH / 3);
			break;
		}
	}
}

void screenManager::renderText(int i)
{
	if (!surfaceMessage[i])
	{

		int w = 0, h = 0;

		initText(i, w, h);

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

void screenManager::renderMenuScreen()
{

	for (int i = 0; i < 2; i++)
	{
		renderText(i);
	}

}

void screenManager::renderLevelChangeScreen()
{
	SDL_SetRenderDrawColor(gameBase::Instance()->gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gameBase::Instance()->gRenderer);

	for (int i = 2; i < 5; i++)
	{
		renderText(i);
	}
}

void screenManager::renderDeathScreen()
{
	SDL_SetRenderDrawColor(gameBase::Instance()->gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gameBase::Instance()->gRenderer);

	renderText(2);
	renderText(5);
}

void screenManager::update()
{
	switch (curScreen) 
	{

		case SCREENS::SCREEN_LEVELCHANGE:
		{
			break;
		}

		case SCREENS::SCREEN_GAMEOVER:
		{
			break;
		}

		default:
		{
			updatePowBlockCollision();
			updateFlagPoleCollision();
			updateEnemyCollision();
			break;
		}

	}
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
			setupLevel(SCREENS::SCREEN_GAMEOVER);
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
			enemys->setDirection(curPosition.x > curEnemyPosition.x ? FACING::LEFT : FACING::RIGHT);
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

			curFlagPole->updateTexture(-(dest.y - curFlagPole->getRenderBox().y - (curFlagPole->getRenderBox().h / 2)));

			if (!(dest.y > (curFlagPole->getRenderBox().y + (32 * 5))))
			{
				highscores::Instance()->curScore++;
			}

			LocalPlayer->setPosition(dest);

			if (dest.y > (curFlagPole->getRenderBox().y + (32 * 5)))
			{
				setupLevel(SCREEN_LEVELCHANGE);
			}

			
		}

	}

}