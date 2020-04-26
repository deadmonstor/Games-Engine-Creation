#include "flagpole.h"

#define FlagPoleSize (32 * 7)

flagpole::flagpole(int x, int y)
{
	imgPartRect.x = 249;
	imgPartRect.y = 594;
	imgPartRect.w = 24; // 249 - 273 
	imgPartRect.h = 168; // 594 - 762

	DestR.w = 32;
	DestR.h = FlagPoleSize;
	DestR.x = x;
	DestR.y = y;

	curTexture = texture2D::Instance()->LoadTextureFromFile("Images/flagpole.png");
}

void flagpole::render()
{
	SDL_RenderCopy(gameBase::Instance()->gRenderer, curTexture, &imgPartRect, &DestR);
}


void flagpole::updateTexture(int y)
{

	y += 64;

	for (int i = 6; i-- > 0; )
	{
		if ((((FlagPoleSize - 64) / 6) * i) <= y)
		{
			imgPartRect.x = flagPositions[i];
			break;
		}
	}
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