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

tile::tile(int _x, int _y, string *textureName)
{
    x = _x;
    y = _y;

    curTexture = texture2D::Instance()->LoadTextureFromFile("Images/" + *textureName);

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
}

tiles::~tiles()
{
}

fstream mapFile;
string curLine;
void tiles::loadFromFile(string fileName)
{
    mapFile.open("maps/" + fileName + ".txt");

    if (!mapFile.good())
    {
        cout << "No map found, making file " << endl;
        mapFile.open("maps/" + fileName + ".txt", fstream::in | fstream::out | fstream::trunc);;

    }

    while (getline(mapFile, curLine))
    {
        regex a(R"(\|(.*),(.*)\|(.*)\|(.))");
        smatch matches;

        if (regex_match(curLine, matches, a)) {

            string curTexture = matches[3];

            tile* curTile = new tile(stoi(matches[1]), stoi(matches[2]), &curTexture);
            tileMap[stoi(matches[1])][stoi(matches[2])] = curTile;
        }

    }

    mapFile.close();


}

tiles* tiles::Instance()
{
    if (!mInstance)
        mInstance = new tiles();

    return mInstance;
}

void tiles::wipeTiles()
{
    map<int, map<int, tile*>>::iterator iteratorX;

    for (iteratorX = tileMap.begin(); iteratorX != tileMap.end(); iteratorX++)
    {
        map<int, tile*>::iterator iteratorY;

        for (iteratorY = iteratorX->second.begin(); iteratorY != iteratorX->second.end(); iteratorY++)
        {

            if (iteratorY->second != nullptr)
            {
                delete iteratorY->second;
                tileMap[iteratorX->first][iteratorY->first] = nullptr;
            }

        }

    }
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

        for (iteratorY = iteratorX->second.begin(); iteratorY != iteratorX->second.end(); iteratorY++)
        {

            if (iteratorY->second != nullptr)
            {
                iteratorY->second->render();
            }

        }

    }

}