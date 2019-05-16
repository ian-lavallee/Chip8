#include "Chip8.h"
#include <stdio.h>
#include <stdlib.h>



Chip8::Chip8() : opcode(0), I(0), pc(0), sp(0), delay_timer(0), sound_timer(0), drawFlag(false)
{
	unsigned char chip8_fontset[80] =
	{
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
}


Chip8::~Chip8()
{
}

bool Chip8::GetDrawFlag()
{
	return this->drawFlag;
}

void Chip8::SetDrawFlag(bool flag)
{
	this->drawFlag = flag;
}

void Chip8::Initialize()
{
	pc = 0x200;  // Program counter starts at 0x200
	opcode = 0;      // Reset current opcode	
	I = 0;      // Reset index register
	sp = 0;      // Reset stack pointer

	// Clear display	
	// Clear stack
	// Clear registers V0-VF
	// Clear memory

	// Load fontset
	for (int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

	// Reset timers
}

void Chip8::EmulateCycle()
{
	// Fetch OpCode
	opcode = memory[pc] << 8 | memory[pc + 1];
	// Decode OpCode
	switch (opcode & 0xF000)
	{
	case 0x1000:
		pc = opcode & 0x0FFF;
		break;
	case 0x2000:
		stack[sp++] = pc;
		pc = opcode & 0x0FFF;
		break;
	case 0x3000: // 3XNN Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
		if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
			pc += 4;
		}
		else {
			pc += 2;
		}
		break;
	case 0x4000: // 4XNN Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
		if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
			pc += 4;
		}
		else {
			pc += 2;
		}
		break;
	case 0x5000: // 5XY0 Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
		if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
			pc += 4;
		}
		else {
			pc += 2;
		}
		break;
	case 0x6000: // 6XNN Sets VX to NN.
		V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
		pc += 2;
		break;
	case 0x7000: // 7XNN Adds NN to VX. (Carry flag is not changed)
		V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
		pc += 2;
		break;
	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000: // 8XY0 Sets VX to the value of VY.
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x0001: // 8XY1 Sets VX to VX or VY. (Bitwise OR operation)
			V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4] | V[(opcode & 0x0F00) >> 8]);
			pc += 2;
			break;
		case 0x0002: // 8XY2 Sets VX to VX and VY. (Bitwise AND operation)
			V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4] & V[(opcode & 0x0F00) >> 8]);
			pc += 2;
			break;
		case 0x0003: // 8XY3 Sets VX to VX xor VY.
			V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x00F0) >> 4] ^ V[(opcode & 0x0F00) >> 8]);
			pc += 2;
			break;
		case 0x0004:// 8XY4 Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
			if (V[(opcode & 0x0F00) >> 8] > (0xFF00 - V[(opcode & 0x00F0) >> 4])) 
			{
				V[0x000F] = 1;
			}
			else
			{
				V[0x000F] = 0;
			}
			V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x0005: // 8XY5 VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			// not subtracted then and
			if ((V[(opcode & 0x0F00) >> 8]) < (V[(opcode & 0x00F0) >> 4]))
			{
				V[0x000F] = 0;
			}
			else
			{
				V[0x000F] = 1;
			}
			V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x0006: // 8XY6 Stores the least significant bit of VX in VF and then shifts VX to the right by 1
			V[0x000F] = V[(opcode & 0x0F00) >> 8] & 0x0001;
			V[(opcode & 0x0F00) >> 8] >>= 1; 
			pc += 2;
			break;
		case 0x0007: // 8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			if ((V[(opcode & 0x0F00) >> 8]) > (V[(opcode & 0x00F0) >> 4]))
			{
				V[0x000F] = 0;
			}
			else
			{
				V[0x000F] = 1;
			}
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x000E: // 8XYE Stores the most significant bit of VX in VF and then shifts VX to the left by 1
			V[0x000F] = V[(opcode & 0x0F00) >> 8] & 0x8000;
			V[(opcode & 0x0F00) >> 8] <<= 1;
			pc += 2;
			break;
		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			break;
		}
	case 0x0000:
		switch (opcode & 0x000F)
		{
		case 0x0000: // 00E0 clear screen
			for (int i = 0; i < 64 * 32; ++i) {
				gfx[i] = 0;
			}
			this->drawFlag = true;
			break;
		case 0x000E: // 00EE return from subroutine
		
		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			break;
		}
	case 0x9000: // 9XY0 Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
		if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
		{
			pc += 4;
		}
		else
		{
			pc += 2;
		}
		break;
	case 0xA000:
		I = opcode & 0x0FFF;
		pc += 2;
		break;
	case 0xB000: // BNNN Jumps to the address NNN plus V0.
		pc = (opcode & 0x0FFF) + V[0x0000];
		break;
	case 0xC000: // CXNN Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
		V[(opcode & 0x0F00) >> 8] = ((opcode & 0x00FF) & (rand() % 256));
		pc += 2;
		break;
	case 0xD000: // DXYN Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
		// Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the 
		// execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from 
		// set to unset when the sprite is drawn, and to 0 if that doesn’t happen
		int x, y, width, height, overDrawn; // compiler error (VS2019) C2360
		x = V[(opcode & 0x0F00) >> 8];
		y = V[(opcode & 0x00F0) >> 4];
		width = 8;
		height = opcode & 0x000F;
		overDrawn = 0;
		for (int i = 0; i < width; ++i)
		{
			for (int z = 0; z < height; ++z)
			{
				if (gfx[x + i + ((height + z) * 64)] = 1)
				{
					V[0x000F] = 1;
					gfx[x + i + ((height + z) * 64)] = 0;
				}
				else
				{
					gfx[x + i + ((height + z) * 64)] = 1;
				}
			}
		}
		drawFlag = true;
		pc += 2;
		break;
	case 0xE000:
		switch (opcode & 0x000F)
		{
		case 0x000E: // EX9E Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
			if (key[V[(opcode & 0x0F00) >> 8]] == 1)
			{
				pc += 4;
			}
			else
			{
				pc += 2;
			}
			break;
		case 0x0001: // EXA1 Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
			if (key[V[(opcode & 0x0F00) >> 8]] == 1)
			{
				pc += 2;
			}
			else
			{
				pc += 4;
			}
			break;
		default:
			printf("Unknown opcode: 0x%X\n", opcode);
			break;
		}
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007: // FX07 Sets VX to the value of the delay timer.
			V[(opcode & 0x0F00) >> 8] = delay_timer;
			pc += 2;
			break;
		case 0x000A: // FX0A  A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
			bool pressed; // for compiler error
			pressed = false;
			for (int i = 0; i < 16; ++i)
			{
				if (key[i])
				{
					V[(opcode & 0x0F00) >> 8] = i;
					pressed = true;
				}
			}
			if (!pressed)
			{
				return;
			}
			pc += 2;
			break;
		case 0x0015: // FX15 Sets the delay timer to VX.
			delay_timer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x0018: // FX18 Sets the sound timer to VX.
			sound_timer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x001E: // FX1E Adds VX to I.
			// maybe should check for overflow and set VF to 1?
			I += V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x0029: // FX29 Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
			I = V[(opcode & 0x0F00) >> 8] * 0x5;
			pc += 2;
			break;
		case 0x0033: // FX33 Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1,
			// and the least significant digit at I plus 2. 
			int val; // for VS compiler error C2360
			val = V[(opcode & 0x0F00) >> 8];
			memory[I] = val / 100;
			memory[I + 1] = (val % 100) / 10;
			memory[I + 2] = val % 10;
			pc += 2;
			break;
		case 0x0055: // FX55 Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
			for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
			{
				memory[I + i] = V[i];
			}
			pc += 2;
			break;
		case 0x0065: // FX65 Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
			for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
			{
				V[i] = memory[I + i];
			}
			pc += 2;
			break;
		default:
			printf("Unknown opcode: 0x%X\n", opcode);
			break;
		}
	default:
		printf("Unknown opcode: 0x%X\n", opcode);
		break;
	}
	// Execute OpCode

	// Update Timers
	if (delay_timer > 0)
	{
		--delay_timer;
	}
	if (sound_timer > 0)
	{
		--sound_timer;
	}
	if (sound_timer == 0)
	{
		playSound = true;
	}
}

bool Chip8::GetPlaySound()
{
	return this->playSound;
}
