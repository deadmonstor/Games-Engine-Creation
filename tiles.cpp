#include "tiles.h"


/*
    =======================TILE=======================
*/

tile::tile(int _x, int _y)
{
    x = _x;
    y = _y;

    curTexture = texture2D::Instance()->LoadTextureFromFile("Images/tile.png");

}

void tile::render()
{
    imgPartRect.x = 0;
    imgPartRect.y = 0;
    imgPartRect.w = 32;
    imgPartRect.h = 32;

    DestR.w = 32;
    DestR.h = 32;
    DestR.x = x * 32;
    DestR.y = y * 32;


    SDL_RenderCopyEx(gameBase::Instance()->gRenderer, curTexture, &imgPartRect, &DestR, NULL, NULL, SDL_FLIP_NONE);
}

/*
    =======================TILES=======================
*/


tiles* tiles::mInstance = NULL;

tiles::tiles()
{
    for (int x = 0; x < 100; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            tile* tiles = new tile(x, y);
            tileMap[x].insert(make_pair(y, tiles));
        }
    }
}

tiles::~tiles()
{

}


tiles* tiles::Instance()
{
    if (!mInstance)
        mInstance = new tiles;

    return mInstance;
}

void tiles::render()
{

    map<int, map<int, tile*>>::iterator iteratorX;

    for (iteratorX = tileMap.begin(); iteratorX != tileMap.end(); iteratorX++)
    {
        map<int, tile*>::iterator iteratorY;

        for (iteratorY = iteratorX->second.begin(); iteratorY != iteratorX->second.end(); iteratorY++) // MAX: 20
        {

            if (iteratorY->second != nullptr)
            {
                iteratorY->second->render();
            }

        }

    }

}