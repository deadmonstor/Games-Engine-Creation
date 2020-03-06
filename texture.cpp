#include "texture.h"

static gameBase* game;


texture2D::texture2D(gameBase* gameBases)
{
	game = gameBases;
}

texture2D::~texture2D()
{

	for (map<string, SDL_Texture*>::const_iterator it = textureList.begin(); it != textureList.end(); ++it)
	{

		FreeTexture(it->second);

	}

}


/*

	Texture - Renderer

*/

SDL_Texture* texture2D::LoadTextureFromFile(string path)
{

	for (map<string, SDL_Texture*>::const_iterator it = textureList.begin(); it != textureList.end(); ++it)
	{

		if (path == it->first) {

			return it->second;

		}

	}

	SDL_Texture* pTexture = NULL;

	SDL_Surface* pSurface = IMG_Load(path.c_str());

	if (pSurface != NULL)
	{
		pTexture = SDL_CreateTextureFromSurface(game->gRenderer, pSurface);

		if (pTexture == NULL)
		{
			printf("Unable to create texture: %s\n", SDL_GetError());
			return NULL;
		}

	}
	else
	{
		printf("Unable to load image from path: %s\n", IMG_GetError());
		return NULL;
	}

	SDL_FreeSurface(pSurface);

	textureList[path] = pTexture;

	return pTexture;

}

void texture2D::FreeTexture(SDL_Texture* texture)
{
	string path;
	for (map<string, SDL_Texture*>::const_iterator it = textureList.begin(); it != textureList.end(); ++it)
	{

		if (texture == it->second) {

			path = it->first;
			break;

		}

	}

	if (path != "")
	{

		SDL_DestroyTexture(texture);
		texture = NULL;
		textureList[path] = NULL;

	}

}