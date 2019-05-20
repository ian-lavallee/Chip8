/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <stdio.h>
#include <iostream>
#include "chip8.h"
#include "Graphics.h"
#include "Input.h"
#include <fstream>
#include <vector>
#include <thread>         
#include <chrono> 

Chip8 cpu;
Graphics graphics;
Input input;
void load();

int main( int argc, char* args[] )
{
	cpu = Chip8();
	graphics = Graphics();
	input = Input();
	
	
	// Initialize the Chip8 system and load the game into the memory
	cpu.Initialize();
	load();

	graphics.Initialize();
	input.Initialize();

	// Emulation loop
	while(true)
	{
		// Emulate one cycle
		cpu.EmulateCycle();

		// If the draw flag is set, update the screen
		if (cpu.GetDrawFlag())
		{
			graphics.Draw(cpu.gfx);
			cpu.SetDrawFlag(false);
		}
			
		
		if (cpu.GetPlaySound())
		{
			// add sound
			
		}

		if (input.quit)
		{
			break;
		}

		// Store key press state (Press and Release)
		input.SetKeys(cpu.key);
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
	
	return 0;
}

void load()
{
	std::vector<char> buffer;
	std::ifstream f("./PONG", std::ios::in | std::ios::binary | std::ios::ate);
	int fsize = f.tellg();
	f.seekg(std::ios::beg);
	buffer.resize(fsize);
	f.read(buffer.data(), fsize);
	f.close();

	cpu.LoadProgram(buffer);
}