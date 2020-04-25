#include "enemy.h"


void enemy::render(SDL_Event _)
{
	SDL_RenderCopyEx(gameBase::Instance()->gRenderer, curTexture, &imgPartRects, &DestRs, NULL, NULL, curFacing == FACING::RIGHT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
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

	if (canMove && (curTile != nullptr && !collisions::Instance()->Box(self, curTile) || canMove && curTile == nullptr))
	{

		float dist = round(50 * deltaTime);
		if (curFacing == FACING::RIGHT)
		{
			DestRs.x = round(DestRs.x + dist);
		}
		else 
		{
			DestRs.x = round(DestRs.x - dist);
		}

	}
	else if(canMove)
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

		DestRs.y += (200.0f * *(float*)event.user.data1);
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

enemy::enemy()
{
	imgPartRects.x = 96;
	imgPartRects.y = 8;
	imgPartRects.w = 15;
	imgPartRects.h = 23;

	DestRs.x = 100;
	DestRs.y = 0;
	DestRs.w = 15 * 2;
	DestRs.h = 23 * 2;

	gameBase::Instance()->hookFunctionEnemies[RENDERUPDATE].push_back(&renderEnemy);
	gameBase::Instance()->hookFunctionEnemies[POSTTICK].push_back(&updateEnemy);

	curTexture = texture2D::Instance()->LoadTextureFromFile("Images/enemies.png");
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