#include "../include/cpu.h"

using namespace component;

cpu::cpu(char* memory, char* registers, bool* flags){
	pc = 0x100;
	this->memory = memory;
	this->registers = registers;
	this->F = flags;
	this->alu = new component::alu(memory, registers, flags);
}

void cpu::execute(int opcode){
	if(!alu->execute(opcode)){
		// if ALU doesn't do it, do it here
	}
}
