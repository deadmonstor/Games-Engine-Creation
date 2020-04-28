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

tile::tile(int _x, int _y, string textureName)
{
    x = _x;
    y = _y;

    curTexture = texture2D::Instance()->LoadTextureFromFile("Images/" + textureName);

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


tiles* tiles::mInstance = tiles::mInstance == nullptr ? NULL : tiles::mInstance;

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

    if (mapFile.fail())
    {
        cout << "No map found, making file " << endl;
        mapFile.open("maps/" + fileName + ".txt", fstream::in | fstream::out | fstream::trunc);;

    }

    while (getline(mapFile, curLine))
    {
        regex a(R"(\|(.*),(.*)\|(.*)\|(.))");
        smatch matches;

        if (regex_match(curLine, matches, a)) 
        {

            int i = stoi(matches[4]);

            switch (i)
            {
                case 3:
                {
                    tile* curTile = new tile(stoi(matches[1]), stoi(matches[2]), matches[3]);
                    tileMap[stoi(matches[1])][stoi(matches[2])] = curTile;
                    curTile->destructable = true;
                }
                break;
                case 1:
                {
                    tile* curTile = new tile(stoi(matches[1]), stoi(matches[2]), matches[3]);
                    tileMap[stoi(matches[1])][stoi(matches[2])] = curTile;
                }
                break;
                case 2:
                {
                    powblock* curPow = new powblock(stoi(matches[1]), stoi(matches[2]));
                    screenManager::Instance()->powTable.push_back(curPow);
                }
                break;
                case 4:
                {
                    enemy* curEnemy = new enemy(stoi(matches[1]), stoi(matches[2]));
                    screenManager::Instance()->enemyTable.push_back(curEnemy);
                }
                break;

            }
           
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

void tiles::setTile(int x, int y)
{
    tileMap[x][y] = nullptr;;
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