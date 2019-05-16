#pragma once
class Chip8 {
private:
	bool drawFlag;
	bool playSound;
	unsigned short opcode;
	unsigned short I;
	unsigned short pc;
	unsigned short stack[16];
	unsigned short sp;
	unsigned char key[16];
	unsigned char gfx[64 * 32];
	unsigned char chip8_fontset[80];
	unsigned char delay_timer;
	unsigned char sound_timer;
	unsigned char memory[4096];
	unsigned char V[16];
public:
	Chip8();
	void Initialize();
	void EmulateCycle();
	bool GetDrawFlag();
	bool GetPlaySound();
	void SetDrawFlag(bool flag);
	~Chip8();
	
};
