#include "enemy.h"


void enemy::render(SDL_Event _)
{
	SDL_RendererFlip curFlip = (curFacing == RIGHT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

	if (isDistrupted)
		curFlip = static_cast<SDL_RendererFlip>(static_cast<int>(curFlip) | static_cast<int>(SDL_FLIP_VERTICAL));

	SDL_RenderCopyEx(gameBase::Instance()->gRenderer, curTexture, &imgPartRects, &DestRs, NULL, NULL, curFlip);
}

static void renderEnemy(SDL_Event event, void* this_pointer)
{
	enemy* self = static_cast<enemy*>(this_pointer);
	self->render(event);
}

static void updateEnemy(SDL_Event event, void* this_pointer)
{
	enemy* self = static_cast<enemy*>(this_pointer);
	self->update(event);
}

void enemy::update(SDL_Event curEvent)
{
	pair<int, int> getTile = getTilePos(curFacing == FACING::RIGHT ? 32 : -32, 0);
	int curTileX = get<0>(getTile), curTileY = get<1>(getTile);

	if (curTileX < 0 || curTileX > (SCREEN_WIDTH / 32))
	{
		curFacing = (curFacing == FACING::LEFT ? FACING::RIGHT : FACING::LEFT);
		return;
	}

	tile* curTile = tiles::Instance()->getTileMap()->at(curTileX).at(curTileY);

	enemy* self = static_cast<enemy*>(this);

	float deltaTime = *(float *)curEvent.user.data1;

	bool canMoveProperly = (canMove && (!isDistrupted && !moveDistrupted || isDistrupted && moveDistrupted));

	if (canMoveProperly && (curTile != nullptr && !collisions::Instance()->Box(self, curTile)
		|| canMoveProperly && curTile == nullptr))
	{

		float dist = round((moveDistrupted ? 250 : 100) * deltaTime);
		if (curFacing == FACING::RIGHT)
		{
			DestRs.x = round(DestRs.x + dist);
		}
		else 
		{
			DestRs.x = round(DestRs.x - dist);
		}

	}
	else if(canMoveProperly)
	{
		curFacing = (curFacing == FACING::LEFT ? FACING::RIGHT : FACING::LEFT);
	}

	addGravity(curEvent, deltaTime);
}

void enemy::addGravity(SDL_Event event, float deltaTime)
{

	pair<int, int> getTileBelow = getTilePos(0, 32);
	int curPosTileUnderX = get<0>(getTileBelow), curPosTileUnderY = get<1>(getTileBelow);

	tile* curTileUnder = tiles::Instance()->getTileMap()->at(curPosTileUnderX).at(curPosTileUnderY);

	enemy* self = static_cast<enemy*>(this);

	canMove = false;

	if (DestRs.y < (SCREEN_HEIGHT - 60))
	{
		if (curTileUnder != nullptr && collisions::Instance()->Box(self, curTileUnder))
		{
			canMove = true;
			return;
		}

		DestRs.y += round(300.0f * *(float*)event.user.data1);
	}
	else 
	{
		canMove = true;
	}
}

pair<int, int> enemy::getTilePos()
{
	int curPosX = round((DestRs.x / 32.0)), curPosY = round((DestRs.y / 32.0));

	return make_pair(curPosX, curPosY);
}

pair<int, int> enemy::getTilePos(int x, int y)
{
	int curPosX = round((DestRs.x + x) / 32.0), curPosY = round(((DestRs.y + y) / 32.0));

	return make_pair(curPosX, curPosY);
}

enemy::enemy(int x, int y)
{
	imgPartRects.x = 96;
	imgPartRects.y = 8;
	imgPartRects.w = 15;
	imgPartRects.h = 23;

	DestRs.x = x;
	DestRs.y = y;
	DestRs.w = 15 * 2;
	DestRs.h = 23 * 2;

	gameBase::Instance()->hookFunctionEnemies.clear();

	gameBase::Instance()->hookFunctionEnemies[RENDERUPDATE].push_back(&renderEnemy);
	gameBase::Instance()->hookFunctionEnemies[PRETICK].push_back(&updateEnemy);

	curTexture = texture2D::Instance()->LoadTextureFromFile("Images/enemies.png");
}

bool enemy::getDistrupted()
{
	return isDistrupted;
}

void enemy::setDistrupted(bool _isDistrupted)
{

	if (_isDistrupted)
	{
		imgPartRects.x = 175;
		imgPartRects.y = 17;
		imgPartRects.w = 16;
		imgPartRects.h = 24;
	}
	else 
	{
		imgPartRects.x = 96;
		imgPartRects.y = 8;
		imgPartRects.w = 15;
		imgPartRects.h = 24;
	}

	isDistrupted = _isDistrupted;
}

bool enemy::getMoveDistrupted()
{
	return moveDistrupted;
}

void enemy::setMoveDistrupted(bool _isDistrupted)
{

	if (_isDistrupted)
	{
		imgPartRects.x = 160; // 160 - 175
		imgPartRects.y = 17;  // 9 - 33
		imgPartRects.w = 15;
		imgPartRects.h = 24;
	}
	else
	{
		imgPartRects.x = 176; // 176 - 191
		imgPartRects.y = 17;  // 9 - 33
		imgPartRects.w = 15;
		imgPartRects.h = 24;
	}

	lastChange = SDL_GetTicks();
	moveDistrupted = _isDistrupted;
}

void enemy::setDirection(FACING _curDirection)
{
	curFacing = _curDirection;
}

Uint32 enemy::getLastChange()
{
	return lastChange;
}

bool enemy::getCanMove()
{
	return canMove;
}

SDL_Rect enemy::getPosition()
{
	return DestRs;
}

SDL_Rect enemy::getRenderBox()
{
	SDL_Rect returnValue;

	returnValue.x = DestRs.x;
	returnValue.y = DestRs.y;
	returnValue.w = DestRs.w * 1.5;
	returnValue.h = DestRs.h;

	return returnValue;
}

enemy::~enemy()
{

}