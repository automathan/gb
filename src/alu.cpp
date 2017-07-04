#include "../include/alu.h"

enum flags {Z_FLAG, N_FLAG, H_FLAG, C_FLAG};
enum reg_index {A, B, C, D, E, H, L};

using namespace component;

alu::alu(unsigned char* memory,unsigned char* registers, bool* flags){
	this->memory = memory;
	this->registers = registers;
	this->F = flags;
};

void alu::add_reg_to_A(int r, bool adc){
	if(registers[A] & 0x8 && registers[r] & 0x8) F[H_FLAG] = 1;
	if(registers[A] & 0x80 && registers[r] & 0x80) F[C_FLAG] = 1;
	F[N_FLAG] = 0;
	registers[A] += registers[r] + (adc ? F[C_FLAG] : 0);
	if(!registers[A]) F[Z_FLAG] = 1;
}

void alu::and_reg_to_A(int r){
	F[N_FLAG] = 0;
	F[H_FLAG] = 1;
	F[C_FLAG] = 0;
	registers[A] &= registers[r];
	if(!registers[A]) F[Z_FLAG] = 1;
}

void alu::or_reg_to_A(int r){
	F[N_FLAG] = 0;
	F[H_FLAG] = 0;
	F[C_FLAG] = 0;
	registers[A] |= registers[r];
	if(!registers[A]) F[Z_FLAG] = 1;
}

void alu::xor_reg_to_A(int r){
	F[N_FLAG] = 0;
	F[H_FLAG] = 0;
	F[C_FLAG] = 0;
	registers[A] ^= registers[r];
	if(!registers[A]) F[Z_FLAG] = 1;
}

void alu::inc_reg(int r){
	if(registers[A] & 0x8 && registers[r] & 0x8) F[H_FLAG] = 1;
	F[N_FLAG] = 0;
	registers[A]++;
	if(!registers[A]) F[Z_FLAG] = 1;
}

bool alu::execute(int opcode){
	if((opcode >= 0x80 && opcode <= 0x85) || opcode == 0x87)
		add_reg_to_A(((opcode % 10) + 1) % 8, false);
	else if((opcode >= 0x88 && opcode <= 0x8D) || opcode == 0x8F)
		add_reg_to_A(((opcode & 0xf) - 7) % 8, true);
	else if((opcode >= 0xA0 && opcode <= 0xA5) || opcode == 0xA7)
		and_reg_to_A(((opcode % 10) + 1) % 8);
	else if((opcode >= 0xB0 && opcode <= 0xB5) || opcode == 0xB7)
		or_reg_to_A(((opcode % 10) + 1) % 8);
	else if((opcode >= 0xA8 && opcode <= 0xAD) || opcode == 0xAF)
		xor_reg_to_A(((opcode & 0xf) - 7) % 8);
	else if((opcode >= 0x04 && opcode <= 0x2C && ((opcode + 4) % 8 == 0)) || opcode == 0x3C)
		inc_reg(((opcode + 4) >> 2) % 8);
	else
		return false;
	return true;
}
