#include "../include/cpu.h"

using namespace component;
enum reg_index {A, B, C, D, E, H, L, F};
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

unsigned char* cpu::getRegs(){
    return registers;
}

unsigned short cpu::getPC(){
    return pc;
}

void cpu::execute(int opcode){
    // NOTES:
    // short registers are little-endian (DE is actually E(b15-8) D(b7-0));
    // everything is little-endian

    // temporary ugly switchcase
    memory[0] = 0; // used now for detecting unimplemented operations

    switch(opcode){
    case 0x00:{ // NOP
        ++pc;
        break;
    } // DONE

    case 0x01:{ // LD BC, nn | load nn into BC (endianess???)

        break;
    }

    case 0x02:{ // LD (BC), A | save val A into (BC)

        break;
    }

    case 0x03:{ // inc BC

        break;
    }

    case 0x04:{ // inc B
        registers[B]++;
        ++pc;
        break;
    }

    case 0x05:{ // dec B
        registers[B]--;
        ++pc;
        break;
    }

    case 0x06:{ // load n into B
        registers[B] = memory[pc + 1];
        pc += 2;
        break;
    }

    case 0x07:{ // RLC (Rotate Left Carry) A

        break;
    }

    case 0x08:{ // LD (nn), SP | sace SP to (nn)

        break;
    }

    case 0x09:{ // ADD HL, BC | add BC to HL

        break;
    }

    case 0x0A:{ // LD A, (BC) | load A from (BC)

        break;
    }

    case 0x0B:{ // dec BC

        break;
    }

    case 0x0C:{ // inc C
        ++registers[C];
        ++pc;
        break;
    }

    case 0x0D:{ // dec C
        --registers[C];
        ++pc;
        break;
    }

    case 0x0E:{ // LD C, n | Load n into C
        registers[C] = memory[pc + 1];
        pc += 2;
        break;
    }

    case 0x0F:{ // RRC A

        break;
    }

    case 0x10:{ // HALT until button press (disabled for now)
        ++pc;
        break;
    }

    case 0x12:{ // put value A into (DE)
        memory[(((unsigned short)registers[E]) << 8) | registers[D]] = registers[A];
        ++pc;
        break;
    }

    case 0x1e:{ // load n into E
        registers[E] = memory[pc + 1];
        pc += 2;
        break;
    } // DONE

    case 0x1f:{ // RR A (Rotate Right)
        ++pc;
        break;
    }

    case 0x20:{ // jump n instructions forward if Z is true
        if(F[Z_FLAG])
            pc += memory[pc + 1];
        else
            pc += 2;
        break;
    } // DONE

    case 0x21:{ // put value nn into HL
        registers[H] = memory[pc + 1];
        registers[L] = memory[pc + 2];

        pc += 3;
        break;
    } // DONE - could be wrong based on endianess, but since both are little, they should be compatible

    case 0x23:{ // HL++
        registers[H]++;
        if(!registers[H])
            registers[L]++;
        ++pc;
        break;
    }case 0x27:{ // DAA Decimal Adjust A
        unsigned char tmp = registers[A];
        registers[A] = (tmp % 10) | (((tmp / 10) % 10) << 4);
        ++pc;
        break;
    }case 0x32:{ // LDD (HL),A | Put value A into (HL), HL--;
        // TODO
        ++pc;
        break;
    }

    case 0x7a:{ // LD A, D
        registers[A] = registers[D];
        ++pc;
        break;
    } // DONE

    case 0xaf:{ // A XOR A
        registers[A] = 0;
        ++pc;
        break;
    } // DONE

    case 0xc3:{ // JP nn
        pc = (((unsigned short)memory[pc + 2]) << 8) | memory[pc + 1]; // first byte has LSB
        break;
    } // DONE

    case 0xe0:{ // put value A into memory location 0xff00 + n
        memory[0xff00 + memory[pc + 1]] = registers[A];

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
