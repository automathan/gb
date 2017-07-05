#include "../include/cpu.h"

using namespace component;
enum reg_index {A, B, C, D, E, H, L};
enum flags {Z_FLAG, N_FLAG, H_FLAG, C_FLAG};

cpu::cpu(unsigned char* memory, unsigned char* registers, bool* flags){
    pc = 0x100;
    this->memory = memory;
    this->registers = registers;
    this->F = flags;
    this->alu = new component::alu(memory, registers, flags);
}

void cpu::step(){
    execute(memory[pc]);
}

unsigned short cpu::getPC(){
    return pc;
}

void cpu::execute(int opcode){
    // temporary ugly switchcase
    memory[0] = 0; // used now for detecting unimplemented operations
    //memory[1] is used for implemented opcodes

    // memory[2] = n_params
    // memory[3] = param 1;
    // memory[4] = param 2;
    memory[2] = 0;

    switch(opcode){
    case 0x05:{ // dec B
        registers[B]--;

        pc++;
        break;
    }case 0x06:{ // load n into B
        registers[B] = memory[pc + 1];

        memory[2] = 1;
        memory[3] = memory[pc + 1];

        pc += 2;
        break;
    }case 0x10:{ // HALT until button press (disabled for now)
        ++pc;
        break;
    }case 0x12:{ // put value A into (DE)
        memory[(((unsigned short)registers[E]) << 8) | registers[D]] = registers[A];
        ++pc;
        break;
    }case 0x1e:{ // load n into E
        registers[E] = memory[pc + 1];

        memory[2] = 1;
        memory[3] = memory[pc + 1];

        pc += 2;
        break;
    }case 0x20:{ // jump n instructions forward if Z is true
        memory[2] = 1;
        memory[3] = memory[pc + 1];

        if(F[Z_FLAG])
            pc += memory[pc + 1];
        else
            pc += 2;
        break;
    }case 0x21:{ // put value nn into HL
        registers[H] = memory[pc + 1];
        registers[L] = memory[pc + 2];

        memory[2] = 2;
        memory[3] = memory[pc + 1];
        memory[4] = memory[pc + 2];

        pc += 3;
        break;
    }case 0x23:{ // HL++
        registers[L]++;
        if(!registers[L])
            registers[H]++;
        ++pc;
        break;
    }case 0x27:{ // DAA Decimal Adjust A
        unsigned char tmp = registers[A];
        registers[A] = (tmp % 10) | (((tmp / 10) % 10) << 4);
        ++pc;
        break;
    }case 0x32:{ // Put value A into (HL), HL--;
        // TODO
        ++pc;
        break;
    }case 0xaf:{ // A ^= A
        registers[A] = 0; // xor self is always 0
        ++pc;
        break;
    }case 0xc3:{ // JP nn
        pc = (((unsigned short)memory[pc + 2]) << 8) | memory[pc + 1];

        memory[2] = 2;
        memory[3] = memory[pc + 1];
        memory[4] = memory[pc + 2];

        break;
    }case 0xe0:{ // put value A into memory location 0xff00 + n
        memory[0xff00 + memory[pc + 1]] = registers[A];

        memory[2] = 1;
        memory[3] = memory[pc + 1];

        pc += 2;
        break;
    }default:
        memory[0] = opcode;
        ++pc;
        break;
    }

    memory[1] = opcode;
    /*
    if(!alu->execute(opcode)){
        // if ALU doesn't do it, do it here
    }
    */
}
