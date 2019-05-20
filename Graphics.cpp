#include "Graphics.h"
#include <iostream>

int width = 1280;
int height = 720;

Graphics::Graphics()
{
}


Graphics::~Graphics()
{
	SDL_DestroyTexture(t);
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	t = NULL;
	r = NULL;
	w = NULL;
	SDL_Quit();
}

void Graphics::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Initialization Error: " << SDL_GetError() << std::endl;
		return;
	}
	w = SDL_CreateWindow("C8emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (w == NULL) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return;
	}
	r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
	if (r == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return;
	}
	else {
		SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xFF);
	}
}

void Graphics::Draw(unsigned char gfx[64 * 32]) 
{
	SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(r);

	int w = width / 64;
	int h = height / 32;
	for (int x = 0; x < 32 * 64; ++x) {
		if (gfx[x]) {
			// x, y, width, height
			SDL_Rect fillRect = { (x % 64) * w, (x / 64) * h, w, h };
			SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF); // white
			SDL_RenderFillRect(r, &fillRect);
		}
	}
	SDL_RenderPresent(r);

}
