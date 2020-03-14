#include "character.h"

void character::keyDown(SDL_Event curEvent)
{

	//if (!canMove){ return; }

	switch (curEvent.key.keysym.sym) {
		
		case SDLK_w: {
			DestR.y--;
			break;
		}

		case SDLK_a: {
			DestR.x--;
			curFacing = FACING::LEFT;
			break;
		}

		case SDLK_s: {
			DestR.y++;
			break;
		}

		case SDLK_d: {
			DestR.x++;
			curFacing = FACING::RIGHT;
			break;
		}

	}

}

static void keyDowns(SDL_Event event, void* this_pointer)
{
	character* self = static_cast<character*>(this_pointer);
	self->keyDown(event);
}

void character::renderCharacter(SDL_Event event)
{
	imgPartRect.x = 126;
	imgPartRect.y = 0;
	imgPartRect.w = 40;
	imgPartRect.h = 75;

	DestR.w = 40;
	DestR.h = 75;


	SDL_RenderCopyEx(game->gRenderer, curTexture, &imgPartRect, &DestR, NULL, NULL, curFacing == 1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

static void renderCharacters(SDL_Event event, void* this_pointer)
{
	character* self = static_cast<character*>(this_pointer);
	self->renderCharacter(event);
}

character::character(gameBase* gameBases, texture2D* texture)
{
	game = gameBases;

	game->hookFunctionCharacter[SDL_KEYDOWN].push_back(&keyDowns);
	game->hookFunctionCharacter[RENDERUPDATE].push_back(&renderCharacters);

	curTexture = texture->LoadTextureFromFile("Images/mariospritesheet.png");
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


character::~character()
{

}