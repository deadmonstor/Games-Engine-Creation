#include "enemy.h"

static gameBase* game;
static SDL_Texture* curTexture;
SDL_Rect imgPartRects;
SDL_Rect DestRs;


void renderEnemy(SDL_Event aaaaaaa)
{
	imgPartRects.x = 126;
	imgPartRects.y = 0;
	imgPartRects.w = 40;
	imgPartRects.h = 75;

	DestRs.x = 100;
	DestRs.w = 40;
	DestRs.h = 75;

	SDL_RenderCopy(game->gRenderer, curTexture, &imgPartRects, &DestRs);

	SDL_SetRenderDrawColor(game->gRenderer, (Uint8)255, (Uint8)255, (Uint8)255, (Uint8)255);
}


enemy::enemy(gameBase* gameBases, texture2D* texture)
{
	game = gameBases;
	game->hookFunction[RENDERUPDATE].push_back(&renderEnemy);

	curTexture = texture->LoadTextureFromFile("Images/mariospritesheet.png");
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