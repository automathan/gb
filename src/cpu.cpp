#include "../include/cpu.h"

using namespace component;

cpu::cpu(char* memory,char* registers, bool* flags){
        this->memory = memory;
        this->registers = registers;
        this->F = flags;
        this->alu = new component::alu(memory, registers, flags);
}
