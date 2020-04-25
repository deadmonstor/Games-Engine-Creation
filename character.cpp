#include "character.h"
#include <cmath>

void character::Jump()
{
	if (!jumping && canJump)
	{
		jumpForce = JUMP_FORCE;
		jumping = true;
		canJump = false;
 		soundManager::Instance()->setVolume(1, 0);
		soundManager::Instance()->playSound(1);
	}
}

void character::keyDown(SDL_Event curEvent)
{
	if (ignoreInput || hasDied) return;
	curDown[curEvent.key.keysym.sym] = true;
}

static void keyDowns(SDL_Event event, void* this_pointer)
{
	character* self = static_cast<character*>(this_pointer);
	self->keyDown(event);
}

void character::keyUp(SDL_Event curEvent)
{
	if (ignoreInput || hasDied) return;
	curDown[curEvent.key.keysym.sym] = false;
}

static void keyUps(SDL_Event event, void* this_pointer)
{
	character* self = static_cast<character*>(this_pointer);
	self->keyUp(event);
}

void character::renderCharacter(SDL_Event event)
{
	imgPartRect.x = 126;
	imgPartRect.y = 0;
	imgPartRect.w = 40;
	imgPartRect.h = 75;

	DestR.w = 40;
	DestR.h = 75;


	if (!hasDied)
		SDL_RenderCopyEx(game->gRenderer, curTexture, &imgPartRect, &DestR, NULL, NULL, curFacing == FACING::RIGHT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

static void renderCharacters(SDL_Event event, void* this_pointer)
{
	character* self = static_cast<character*>(this_pointer);
	self->renderCharacter(event);
}

float lerp(float start, float end, float amnt) {
	return start + amnt * (end - start);
}

void character::Update(SDL_Event event)
{
	checkCollisions();

	if (jumping)
	{
		pair<int, int> curPos = getTilePos(0, -32);
		int curPosX = get<0>(curPos), curPosY = get<1>(curPos);

		if (curPosX < 0 || curPosY < 0) 
		{
			jumping = false;
			return;
		}

		if (jumpForce <= 0.0f)
			jumping = false;

		tile* curTile = tiles::Instance()->getTileMap()->at(curPosX).at(curPosY);
		
		character* self = static_cast<character*>(this);

		if (!collisionCache[curPosX][curPosY] || collisionCache[curPosX][curPosY] && curTile != nullptr && !collisions::Instance()->Box(self, curTile) )
		{
			DestR.y = lerp(DestR.y, DestR.y - round(JUMP_FORCE * *(float*)event.user.data1), 0.75f);
		}

		jumpForce = lerp(jumpForce, jumpForce - round(JUMP_FORCE_DECREMENT * *(float*)event.user.data1), 0.75f);

	}
	else {
		addGravity(event, *(float*)event.user.data1);
	}

	
	if (curDown[SDLK_a])
	{
		pair<int, int> getTileLeft = getTilePos(-32, 0);
		int curLeftPosX = get<0>(getTileLeft), curLeftPosY = get<1>(getTileLeft);

		if (curLeftPosX >= 0 && curLeftPosY >= 0)
		{
			tile* curTileLeft = tiles::Instance()->getTileMap()->at(curLeftPosX).at(curLeftPosY);

			character* self = static_cast<character*>(this);

			if (curTileLeft == nullptr || curTileLeft != nullptr && !collisions::Instance()->Box(self, curTileLeft))
			{
				DestR.x -= round(150 * *(float*)event.user.data1);
				curFacing = LEFT;
			}

		}
	}
	
	if (curDown[SDLK_d])
	{

		pair<int, int> getTileLeft = getTilePos(32, 0);
		int curRightPosX = get<0>(getTileLeft), curRightPosZ = get<1>(getTileLeft);

		if ((curRightPosX >= 0 && curRightPosZ >= 0) )
		{

			tile* curTileRight = tiles::Instance()->getTileMap()->at(curRightPosX).at(curRightPosZ);

			character* self = static_cast<character*>(this);

			if (curTileRight == nullptr || curTileRight != nullptr && !collisions::Instance()->Box(self, curTileRight))
			{
				DestR.x += round(150 * *(float*)event.user.data1);
				curFacing = RIGHT;
			}

		};

		
	}

	if (curDown[SDLK_SPACE])
		Jump();

}

void character::cancelJump()
{
	jumping = false;
}

static void updateCharacter(SDL_Event event, void* this_pointer)
{
	character* self = static_cast<character*>(this_pointer);
	self->Update(event);
}

pair<int, int> character::getTilePos()
{
	int curPosX = round((DestR.x / 32.0)), curPosY = round((DestR.y / 32.0));

	return make_pair(curPosX, curPosY);
}

pair<int, int> character::getTilePos(int x, int y)
{
	int curPosX = round((DestR.x + x) / 32.0), curPosY = round(((DestR.y + y) / 32.0));

	return make_pair(curPosX, curPosY);
}

void character::addGravity(SDL_Event event, float deltaTime)
{

	pair<int, int> getTileBelow = getTilePos(0, 32);
	int curPosTileUnderX = get<0>(getTileBelow), curPosTileUnderY = get<1>(getTileBelow);

	tile* curTileUnder = tiles::Instance()->getTileMap()->at(curPosTileUnderX).at(curPosTileUnderY);

	character* self = static_cast<character*>(this);

	canJump = false;
	if (DestR.y < (SCREEN_HEIGHT - 60))
	{
		if (curTileUnder != nullptr && collisions::Instance()->Box(self, curTileUnder))
		{
			canJump = true;
			return;
		}


		DestR.y = lerp(DestR.y, DestR.y + round(300.0f * deltaTime), 0.95f);;
	}
	else
	{
		canJump = true;
	}
}

character::character(gameBase* gameBases, texture2D* texture)
{
	game = gameBases;

	game->hookFunctionCharacter.clear();

	game->hookFunctionCharacter[SDL_KEYDOWN].push_back(&keyDowns);
	game->hookFunctionCharacter[SDL_KEYUP].push_back(&keyUps);

	game->hookFunctionCharacter[RENDERUPDATE].push_back(&renderCharacters);
	game->hookFunctionCharacter[POSTTICK].push_back(&updateCharacter);

	curTexture = texture->LoadTextureFromFile("Images/mariospritesheet.png");

	DestR.y = 40;

	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			collisionCache[x].insert(make_pair(y, false));
		}
	}
}

SDL_Rect character::getPosition()
{
	return DestR;
}

SDL_Rect character::getRenderBox()
{
	SDL_Rect returnValue;
	returnValue.x = DestR.x;
	returnValue.y = DestR.y;
	returnValue.w = DestR.w * 1.5;
	returnValue.h = DestR.h;

	return returnValue;
}


void character::checkCollisions()
{

	pair<int, int> curPos = getTilePos();
	int curPosX = get<0>(curPos), curPosY = get<1>(curPos);

	map<int, map<int, tile*>>* curInstance = tiles::Instance()->getTileMap();

	for (int x = -3; x <= 3; x++) 
	{

		for (int y = -3; y <= 3; y++) {

			int actualX = curPosX - x, actualY = curPosY - y;
			if (curInstance->size() < actualX || curInstance->at(actualX).size() < actualY)
			{
				collisionCache[actualX][actualY] = false;
				continue;
			}

			tile* curTile = curInstance->at(actualX).at(actualY);

			collisionCache[actualX][actualY] = (curTile != NULL);

		}

	}

}

character::~character()
{

}