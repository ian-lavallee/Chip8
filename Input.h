#pragma once

#include "SDL.h"

class Input
{
public:
	Input();
	~Input();
	void Initialize();
	void SetKeys(unsigned char key[16]);
	bool quit = false;
	bool reset = false;
	int speed = 1000; // play around
};

