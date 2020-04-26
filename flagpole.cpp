#include "flagpole.h"


flagpole::flagpole(int x, int y)
{
	imgPartRect.x = 116;
	imgPartRect.y = 594;
	imgPartRect.w = 24; // 116 - 140 
	imgPartRect.h = 168; // 594 - 762

	DestR.w = 32;
	DestR.h = 32 * 7;
	DestR.x = x;
	DestR.y = y;

	curTexture = texture2D::Instance()->LoadTextureFromFile("Images/flagpole.png");
}

void flagpole::render()
{
	SDL_RenderCopy(gameBase::Instance()->gRenderer, curTexture, &imgPartRect, &DestR);
}

SDL_Rect flagpole::getRenderBox()
{
	SDL_Rect copy;

	copy.w = 0;
	copy.h = DestR.h;
	copy.x = DestR.x;
	copy.y = DestR.y;

	return copy;
}