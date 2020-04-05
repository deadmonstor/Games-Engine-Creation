#include "character.h"

void character::Jump()
{
	if (!jumping && canJump)
	{
		jumpForce = JUMP_FORCE;
		jumping = true;
		canJump = false;
 		soundManager::Instance()->setVolume(1, 5);
		soundManager::Instance()->playSound(1);
	}
}

void character::keyDown(SDL_Event curEvent)
{

	curDown[curEvent.key.keysym.sym] = true;

	if (!canMove){ return; }

}

static void keyDowns(SDL_Event event, void* this_pointer)
{
	character* self = static_cast<character*>(this_pointer);
	self->keyDown(event);
}

void character::keyUp(SDL_Event curEvent)
{
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


	SDL_RenderCopyEx(game->gRenderer, curTexture, &imgPartRect, &DestR, NULL, NULL, curFacing == FACING::RIGHT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

static void renderCharacters(SDL_Event event, void* this_pointer)
{
	character* self = static_cast<character*>(this_pointer);
	self->renderCharacter(event);
}

void character::Update(SDL_Event event)
{

	checkCollisions();

	if (jumping)
	{
		DestR.y -= JUMP_FORCE * *(float*)event.user.data1;

		jumpForce -= JUMP_FORCE_DECREMENT * *(float*)event.user.data1;

		if (jumpForce <= 0.0f)
			jumping = false;

	}
	else {
		addGravity(event, (int)event.user.data1);
	}

	
	if (curDown[SDLK_a])
	{
		DestR.x--;
		curFacing = FACING::LEFT;
	}
	else if (curDown[SDLK_d])
	{
		DestR.x++;
		curFacing = FACING::RIGHT;
	}

	if (curDown[SDLK_SPACE])
		Jump();

}

static void updateCharacter(SDL_Event event, void* this_pointer)
{
	character* self = static_cast<character*>(this_pointer);
	self->Update(event);
}

void character::addGravity(SDL_Event event, float deltaTime)
{
	if (DestR.y < (SCREEN_HEIGHT - 60))
	{
		DestR.y += (200.0f * *(float*)event.user.data1);
		canJump = false;
	}
	else
	{
		DestR.y = SCREEN_HEIGHT - 60;
		canJump = true;
	}
}

character::character(gameBase* gameBases, texture2D* texture)
{
	game = gameBases;

	game->hookFunctionCharacter[SDL_KEYDOWN].push_back(&keyDowns);
	game->hookFunctionCharacter[SDL_KEYUP].push_back(&keyUps);

	game->hookFunctionCharacter[RENDERUPDATE].push_back(&renderCharacters);
	game->hookFunctionCharacter[POSTTICK].push_back(&updateCharacter);

	curTexture = texture->LoadTextureFromFile("Images/mariospritesheet.png");

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
	int curPosX = DestR.x / 32, curPosY = DestR.y / 23;

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