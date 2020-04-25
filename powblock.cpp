#include "powblock.h"

powblock::powblock(int x, int y)
{
	imgRect.x = 0;
	imgRect.y = 0;

	positionRect.x = x;
	positionRect.y = y;

	imgRect.w = SIZE;
	imgRect.h = SIZE;
	positionRect.w = SIZE;
	positionRect.h = SIZE;

	mTexture = texture2D::Instance()->LoadTextureFromFile("Images/powblock.png");
}

powblock::~powblock()
{

}

pair<int, int> powblock::getTilePos()
{
	int curPosX = round((positionRect.x / 32.0)), curPosY = round((positionRect.y / 32.0));

	return make_pair(curPosX, curPosY);
}

pair<int, int> powblock::getTilePos(int x, int y)
{
	int curPosX = round((positionRect.x + x) / 32.0), curPosY = round(((positionRect.y + y) / 32.0));

	return make_pair(curPosX, curPosY);
}

void powblock::render()
{
	SDL_RenderCopy(gameBase::Instance()->gRenderer, mTexture, &imgRect, &positionRect);
}

SDL_Rect powblock::getPosition()
{
	return positionRect;
}

void powblock::enemyDisrupt()
{
	for (enemy* curEnemy : screenManager::Instance()->enemyTable) 
	{

		if (curEnemy->getCanMove())
		{
			curEnemy->setMoveDistrupted(false);
			curEnemy->setDistrupted(!curEnemy->getDistrupted());
		}

	}
}

SDL_Rect powblock::getRenderBox()
{
	SDL_Rect returnValue;
	returnValue.x = positionRect.x;
	returnValue.y = positionRect.y;
	returnValue.w = positionRect.w;
	returnValue.h = positionRect.h;

	return returnValue;
}

void powblock::onHit()
{
	if (lastUsed > SDL_GetTicks() - 250)
		return;

	lastUsed = SDL_GetTicks();

	mCurHit++;

	enemyDisrupt();

	switch (mCurHit)
	{
		case 1:
		{
			// Change animation to smaller one
			positionRect.h = 27;
			break;
		}

		case 2:
		{
			// Change animation to an even smaller one
			positionRect.h = 22;
			break;
		}

		case 3:
		{
			// Destroy block

			std::vector<powblock*>::iterator position = std::find(screenManager::Instance()->powTable.begin(), screenManager::Instance()->powTable.end(), this);
			if (position != screenManager::Instance()->powTable.end())
				screenManager::Instance()->powTable.erase(position);

			delete this;
			break;
		}


	}
}

void powblock::onDestroy()
{

}