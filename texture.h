#pragma once
#include "main.h"

#ifndef _TEXTURE_H
	#define _TEXTURE_H

	class texture2D {

	public:
		texture2D(gameBase* gameBases);
		~texture2D();

		map<string, SDL_Texture*> textureList;

		//Texture
		SDL_Texture* LoadTextureFromFile(string path);
		void FreeTexture(SDL_Texture* texture);
	};

#endif