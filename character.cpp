#include "character.h"

static gameBase* game;
static SDL_Texture* curTexture;
SDL_Rect imgPartRect;
SDL_Rect DestR;
bool canMove; 

void keyDownCharacter(SDL_Event curEvent)
{

	//if (!canMove){ return; }

	switch (curEvent.key.keysym.sym) {
		
		case SDLK_w: {
			DestR.y--;
			break;
		}

		case SDLK_a: {
			DestR.x--;
			break;
		}

		case SDLK_s: {
			DestR.y++;
			break;
		}

		case SDLK_d: {
			DestR.x++;
			break;
		}

	}

}


void renderCharacter(SDL_Event aaaaaaa)
{
	imgPartRect.x = 126;
	imgPartRect.y = 0;
	imgPartRect.w = 40;
	imgPartRect.h = 75;

	DestR.w = 40;
	DestR.h = 75;


	SDL_RenderCopyEx(game->gRenderer, curTexture, &imgPartRect, &DestR, NULL, NULL, SDL_FLIP_HORIZONTAL);
}

character::character(gameBase* gameBases, texture2D* texture)
{
	game = gameBases;

	game->hookFunction[SDL_KEYDOWN].push_back(&keyDownCharacter);
	game->hookFunction[RENDERUPDATE].push_back(&renderCharacter);

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