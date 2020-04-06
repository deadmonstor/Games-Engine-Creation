#include "tiles.h"


/*
    =======================TILE=======================
*/

tile::tile(int _x, int _y)
{
    x = _x;
    y = _y;

    curTexture = texture2D::Instance()->LoadTextureFromFile("Images/tile.png");
    
    imgPartRect.w = SIZE; 
    imgPartRect.h = SIZE;
    DestR.w = SIZE;
    DestR.h = SIZE;

}

void tile::render()
{

    DestR.x = x * 32;
    DestR.y = y * 32;


    SDL_RenderCopyEx(gameBase::Instance()->gRenderer, curTexture, &imgPartRect, &DestR, NULL, NULL, SDL_FLIP_NONE);
}

SDL_Rect tile::getCollisionBox()
{
    return DestR;
}

/*
    =======================TILES=======================
*/


tiles* tiles::mInstance = NULL;

tiles::tiles()
{
    for (int x = 0; x < 100; x++)
    {
        for (int y = 0; y < 100; y++)
        {
            tileMap[x].insert(make_pair(y, nullptr));
        }
    }

    for (int x = 0; x < 100; x++)
    {
        tile* tiles = new tile(x, 0);
        tileMap[x][1] = tiles;

        tiles = new tile(x, 5);
        tileMap[x][5] = tiles;
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

map<int, map<int, tile*>>* tiles::getTileMap()
{
    return &tileMap;
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