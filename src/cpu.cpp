#include "../include/cpu.h"

using namespace component;

typedef unsigned char byte;
enum reg_index {A, B, C, D, E, H, L, F};
enum flags {
    Z_FLAG = 0x80,
    N_FLAG = 0x40,
    H_FLAG = 0x20,
    C_FLAG = 0x10
};

cpu::cpu(byte* memory, byte* registers){
    pc = 0x100;
    sp = 0xfffe;
    this->memory = memory;
    this->registers = registers;
}

void cpu::step(){
    execute(memory[pc]);
    ++pc; // REMEMBER TO SUBTRACT 1 FROM JUMPS TO COMPENSATE FOR THIS
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
        break;
    } // DONE

    case 0x01:{ // LD BC, nn | load nn into BC (endianess???)
        registers[B] = memory[pc + 1];
        registers[C] = memory[pc + 2];
        pc += 2;
        break;
    }

    case 0x02:{ // LD (BC), A | save val A into (BC)
        memory[(((unsigned short)registers[C])<<8) | registers[B]] = registers[A];
        break;
    }

    case 0x04:{ // inc B
        registers[B]++;
        break;
    }

    case 0x05:{ // dec B
        registers[B]--;
        break;
    }

    case 0x06:{ // LD B, n
        registers[B] = memory[++pc];
        break;
    } // DONE

    case 0x07:{ // RLC (Rotate Left Carry) A
        registers[A] <<= 1;

        break;
    } // TEMP

    case 0x0C:{ // inc C
        ++registers[C];

        break;
    }

    case 0x0D:{ // dec C
        --registers[C];

        break;
    }

    case 0x0E:{ // LD C, n
        registers[C] = memory[++pc];
        break;
    } // DONE

    case 0x12:{ // put value A into (DE)
        memory[(((unsigned short)registers[E])<<8) | registers[D]] = registers[A];
        break;
    }

    case 0x16:{ // LD D, n
        registers[D] = memory[++pc];
        break;
    } // DONE

    case 0x1e:{ // load n into E
        registers[E] = memory[++pc];
        break;
    } // DONE

    case 0x20:{ // jump n instructions forward if Z is true
        /*if(F[Z_FLAG])
            pc += memory[pc + 1];
        else*/
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

        break;
    }

    case 0x26:{ // LD H, n
        registers[H] = memory[++pc];
        break;
    } // DONE

    case 0x27:{ // DAA Decimal Adjust A
        byte tmp = registers[A];
        registers[A] = (tmp % 10) | (((tmp / 10) % 10) << 4);

        break;
    }

    case 0x2e:{ // LD L, n
        registers[L] = memory[++pc];
        break;
    } // DONE

    case 0x32:{ // LDD (HL),A | Put value A into (HL), HL--;
        // TODO

        break;
    }

    case 0x3e:{ // LD A. n
        registers[A] = memory[++pc];
        break;
    } // DONE

    case 0x47:{ // LD B, A
        registers[B] = registers[A];
        break;
    } // DONE

    case 0x4f:{ // LD C, A
        registers[C] = registers[A];
        break;
    } // DONE

    case 0x57:{ // LD D, A
        registers[D] = registers[A];
        break;
    } // DONE

    case 0x5f:{ // LD E, A
        registers[E] = registers[A];
        break;
    } // DONE

    case 0x67:{ // LD H, A
        registers[H] = registers[A];
        break;
    } // DONE

    case 0x6f:{ // LD L, A
        registers[L] = registers[A];
        break;
    } // DONE

    case 0x77:{
        memory[(((unsigned short)registers[L])<<8) | registers[H]] = registers[A];
        break;
    }

    case 0x7a:{ // LD A, D
        registers[A] = registers[D];

        break;
    } // DONE

    case 0x7f:{
        registers[A] = registers[A]; // kek

        break;
    }

    case 0xa0:{ // A = A AND B
        registers[A] &= registers[B];
        registers[F] = (registers[A] ? Z_FLAG : 0x00) | H_FLAG;
        break;
    } // DONE

    case 0xa1:{ // A = A AND C
        registers[A] &= registers[C];
        registers[F] = (registers[A] ? Z_FLAG : 0x00) | H_FLAG;
        break;
    } // DONE

    case 0xa2:{ // A = A AND D
        registers[A] &= registers[D];
        registers[F] = (registers[A] ? Z_FLAG : 0x00) | H_FLAG;
        break;
    } // DONE

    case 0xa3:{ // A = A AND E
        registers[A] &= registers[E];
        registers[F] = (registers[A] ? Z_FLAG : 0x00) | H_FLAG;
        break;
    } // DONE

    case 0xa4:{ // A = A AND H
        registers[A] &= registers[H];
        registers[F] = (registers[A] ? Z_FLAG : 0x00) | H_FLAG;
        break;
    } // DONE

    case 0xa5:{ // A = A AND L
        registers[A] &= registers[L];
        registers[F] = (registers[A] ? Z_FLAG : 0x00) | H_FLAG;
        break;
    } // DONE

    case 0xa7:{ // A = A AND A
        registers[A] &= registers[A];
        registers[F] = (registers[A] ? Z_FLAG : 0x00) | H_FLAG;
        break;
    } // DONE

    case 0xa8:{ // A = A XOR B
        registers[A] ^= registers[B];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xa9:{ // A = A XOR C
        registers[A] ^= registers[C];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xaa:{ // A = A XOR D
        registers[A] ^= registers[D];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xab:{ // A = A XOR E
        registers[A] ^= registers[E];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xac:{ // A = A XOR H
        registers[A] ^= registers[H];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xad:{ // A = A XOR L
        registers[A] ^= registers[L];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xaf:{ // A = A XOR A
        registers[A] = 0;
        registers[F] = Z_FLAG;
        break;
    } // DONE

    case 0xb0:{ // A = A OR B
        registers[A] |= registers[B];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xb1:{ // A = A OR C
        registers[A] |= registers[C];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xb2:{ // A = A OR D
        registers[A] |= registers[D];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xb3:{ // A = A OR E
        registers[A] |= registers[E];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE


    case 0xb4:{ // A = A OR H
        registers[A] |= registers[H];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE


    case 0xb5:{ // A = A OR L
        registers[A] |= registers[L];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xb7:{ // A = A OR A
        registers[A] = registers[A] | registers[A];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xc3:{ // JP nn
        pc = ((((unsigned short)memory[pc + 2]) << 8) | memory[pc + 1]) - 1;
        break;
    } // DONE

    case 0xe0:{ // LD (0xff00 + n), A
        memory[0xff00 + memory[++pc]] = registers[A];
        break;
    } // DONE

    case 0xe2:{ // LD (0xff00 + C), A
        memory[0xff00 + registers[C]] = registers[A];
        break;
    } // DONE

    case 0xea:{
        memory[(((unsigned short)memory[pc+2])<<8) | memory[pc+1]] = registers[A];
        pc += 2;
        break;
    }

    case 0xee:{ // A = A XOR n
        registers[A] ^= memory[++pc];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xf0:{ // LD A, (0xff00 + n)
        registers[A] = memory[0xff00 + memory[++pc]];
        break;
    } // DONE

    case 0xf2:{ // LD A, (0xff00 + C)
        registers[A] = memory[0xff00 + registers[C]];
        break;
    } // DONE

    case 0xf6:{ // A = A OR n
        registers[A] |= memory[++pc];
        registers[F] = registers[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    default:
        memory[0] = opcode; // to signal unimplemented instructions
        break;
    }

    memory[1] = opcode;
}
