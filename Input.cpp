#include "Input.h"
#include <iostream>

unsigned char keymap[16] =
{
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_4,
	SDLK_q,
	SDLK_w,
	SDLK_e,
	SDLK_r,
	SDLK_a,
	SDLK_s,
	SDLK_d,
	SDLK_f,
	SDLK_z,
	SDLK_x,
	SDLK_c,
	SDLK_v,
};

void Input::Initialize() {
	if (SDL_Init(SDL_INIT_EVENTS) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl; // need to change printfs to cout since im using c++
		return;
	}
}

void Input::SetKeys(unsigned char key[16])
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			quit = true;
		}
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_ESCAPE)
				quit = true;
			if (e.key.keysym.sym == SDLK_DOWN)
				speed += 200;
			if (e.key.keysym.sym == SDLK_UP)
				(speed > 200) ? speed -= 200 : speed = 0;
			if (e.key.keysym.sym == SDLK_RCTRL)
				reset = true;
			for (int i = 0; i < 16; i++) {
				if (e.key.keysym.sym == keymap[i])
					key[i] = true;
			}
		}
		if (e.type == SDL_KEYUP) {
			for (int i = 0; i < 16; i++) {
				if (e.key.keysym.sym == keymap[i])
					key[i] = false;
			}
		}
	}
}

Input::Input()
{
}


Input::~Input()
{
}
