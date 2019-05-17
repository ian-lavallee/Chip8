#pragma once

#include "SDL.h"

class Graphics
{
public:
	Graphics();
	~Graphics();
	SDL_Window* w = NULL;
	SDL_Renderer* r = NULL;
	SDL_Texture* t = NULL;
	void Draw(unsigned char gfx [64 * 32]);
	void Initialize();
};

