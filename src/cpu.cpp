#include "../include/cpu.h"

using namespace component;

enum reg_index {A, B, C, D, E, H, L, F};
enum flags {
    Z_FLAG = 0x80,
    N_FLAG = 0x40,
    H_FLAG = 0x20,
    C_FLAG = 0x10
};

cpu::cpu(byte* mem, byte* reg){
    pc = 0x100;
    sp = 0xfffe;
    this->mem = mem;
    this->reg = reg;
}

void cpu::step(){
    execute(mem[pc]);
    ++pc; // REMEMBER TO SUBTRACT 1 FROM JUMPS TO COMPENSATE FOR THIS
}

void cpu::z80_cp(byte a, byte b){
    reg[F] = (a == b ? Z_FLAG : 0x00) | N_FLAG
            | ((a & 0xf) < (b & 0xf) ? H_FLAG : 0x00)
            | (a < b ? C_FLAG : 0x00);
}

byte cpu::z80_xor(byte a, byte b){
    byte tmp = a ^ b;
    reg[F] = tmp ? 0x00 : Z_FLAG;
    return tmp;
}

byte cpu::z80_or(byte a, byte b){
    byte tmp = a | b;
    reg[F] = tmp ? 0x00 : Z_FLAG;
    return tmp;
}

byte cpu::z80_and(byte a, byte b){
    byte tmp = a & b;
    reg[F] = (tmp ? 0x00 : Z_FLAG) | H_FLAG;
    return tmp;
}

inline void cpu::z80_inc(byte* ptr){
    ++(*ptr);
    reg[F] = (*ptr ? 0x00 : Z_FLAG)
            |(*ptr & 0xf ? 0x00 : H_FLAG)
            |(reg[F] & C_FLAG ? C_FLAG : 0x00);
}

inline void cpu::z80_dec(byte* ptr){
    --(*ptr);
    reg[F] = (*ptr ? 0x00 : Z_FLAG) | N_FLAG
            |((*ptr & 0xf) == 0xf ? H_FLAG : 0x00)
            |(reg[F] & C_FLAG ? C_FLAG : 0x00);
}

unsigned short cpu::pack(byte lsby, byte msby){ // packs two bytes into a short, the arguments are ordered big-endian, but packed little-endian
    return (((unsigned char)msby)<<8) | lsby;
}

unsigned short cpu::getPC(){
    return pc;
}

unsigned short cpu::getSP(){
    return sp;
}

void cpu::execute(int opcode){
    // NOTES:
    // short reg are little-endian (DE is actually E(b15-8) D(b7-0));
    // everything is little-endian

    unimpl = -1;

    switch(opcode){
    case 0x00:{ // NOP
        break;
    } // DONE

    case 0x01:{ // LD BC, nn | load nn into BC (endianess???)
        reg[B] = mem[pc + 1];
        reg[C] = mem[pc + 2];
        pc += 2;
        break;
    }

    case 0x02:{ // LD (BC), A
        mem[pack(reg[B], reg[C])] = reg[A];
        break;
    } // DONE

    case 0x04:{ // inc B
        reg[B]++;
        break;
    }

    case 0x05:{ // DEC B
        z80_dec(&reg[B]);
        break;
    } // DONE

    case 0x06:{ // LD B, n
        reg[B] = mem[++pc];
        break;
    } // DONE

    case 0x07:{ // RLC (Rotate Left Carry) A
        reg[A] <<= 1;

        break;
    } // TEMP

    case 0x0a:{ // LD A, (BC)
        reg[A] = mem[pack(reg[B], reg[C])];
        break;
    } // DONE

    case 0x0C:{ // inc C
        ++reg[C];

        break;
    }

    case 0x0d:{ // DEC C
        z80_dec(&reg[C]);
        break;
    } // DONE

    case 0x0E:{ // LD C, n
        reg[C] = mem[++pc];
        break;
    } // DONE

    case 0x12:{ // LD (DE), A
        mem[pack(reg[D], reg[E])] = reg[A];
        break;
    } // DONE

    case 0x15:{
        z80_dec(&reg[D]);
        break;
    } // DONE

    case 0x16:{ // LD D, n
        reg[D] = mem[++pc];
        break;
    } // DONE

    case 0x1a:{ // LD A, (DE)
        reg[A] = mem[pack(reg[D], reg[E])];
        break;
    } // DONE

    case 0x1e:{ // load n into E
        reg[E] = mem[++pc];
        break;
    } // DONE

    case 0x20:{ // jump n instructions forward if Z is true
        if(reg[F] & Z_FLAG)
            pc += mem[pc + 1] - 1;
        else
            pc += 2;
        break;
    } // DONE

    case 0x21:{ // put value nn into HL
        reg[H] = mem[pc + 1];
        reg[L] = mem[pc + 2];
        pc += 2;
        break;
    } // DONE - could be wrong based on endianess, but since both are little, they should be compatible

    case 0x26:{ // LD H, n
        reg[H] = mem[++pc];
        break;
    } // DONE

    case 0x27:{ // DAA Decimal Adjust A
        byte tmp = reg[A];
        reg[A] = (tmp % 10) | (((tmp / 10) % 10) << 4);

        break;
    }

    case 0x2e:{ // LD L, n
        reg[L] = mem[++pc];
        break;
    } // DONE

    case 0x3d:{ // DEC A
         z80_dec(&reg[A]);
         break;
    } // DONE

    case 0x3e:{ // LD A. n
        reg[A] = mem[++pc];
        break;
    } // DONE

    case 0x40:{ // LD B, B
        reg[B] = reg[B];
        break;
    } // DONE

    case 0x41:{ // LD B, C
        reg[B] = reg[C];
        break;
    } // DONE

    case 0x42:{ // LD B, D
        reg[B] = reg[D];
        break;
    } // DONE

    case 0x43:{ // LD B, E
        reg[B] = reg[E];
        break;
    } // DONE

    case 0x44:{ // LD B, H
        reg[B] = reg[H];
        break;
    } // DONE

    case 0x45:{ // LD B, L
        reg[B] = reg[L];
        break;
    } // DONE

    case 0x46:{ // LD B, (HL)
        reg[B] = mem[pack(reg[H], reg[L])];
        break;
    } // DONE

    case 0x47:{ // LD B, A
        reg[B] = reg[A];
        break;
    } // DONE

    case 0x4f:{ // LD C, A
        reg[C] = reg[A];
        break;
    } // DONE

    case 0x50:{ // LD D, B
        reg[D] = reg[B];
        break;
    } // DONE

    case 0x51:{ // LD D, C
        reg[D] = reg[C];
        break;
    } // DONE

    case 0x52:{ // LD D, D
        reg[D] = reg[D];
        break;
    } // DONE

    case 0x53:{ // LD D, E
        reg[D] = reg[E];
        break;
    } // DONE

    case 0x54:{ // LD D, H
        reg[D] = reg[H];
        break;
    } // DONE

    case 0x55:{ // LD D, L
        reg[D] = reg[L];
        break;
    } // DONE

    case 0x56:{ // LD D, (HL)
        reg[D] = mem[pack(reg[H], reg[L])];
        break;
    } // DONE

    case 0x57:{ // LD D, A
        reg[D] = reg[A];
        break;
    } // DONE

    case 0x5f:{ // LD E, A
        reg[E] = reg[A];
        break;
    } // DONE

    case 0x60:{ // LD H, B
        reg[H] = reg[B];
        break;
    } // DONE

    case 0x61:{ // LD H, C
        reg[H] = reg[C];
        break;
    } // DONE

    case 0x62:{ // LD H, D
        reg[H] = reg[D];
        break;
    } // DONE

    case 0x63:{ // LD H, E
        reg[H] = reg[E];
        break;
    } // DONE

    case 0x64:{ // LD H, H
        reg[H] = reg[H];
        break;
    } // DONE

    case 0x65:{ // LD H, L
        reg[H] = reg[L];
        break;
    } // DONE

    case 0x66:{ // LD H, (HL)
        reg[H] = mem[pack(reg[H], reg[L])];
        break;
    } // DONE

    case 0x67:{ // LD H, A
        reg[H] = reg[A];
        break;
    } // DONE

    case 0x68:{ // LD L, B
        reg[L] = reg[B];
        break;
    } // DONE

    case 0x69:{ // LD L, C
        reg[L] = reg[C];
        break;
    } // DONE

    case 0x6a:{ // LD L, D
        reg[L] = reg[D];
        break;
    } // DONE

    case 0x6b:{ // LD L, E
        reg[L] = reg[E];
        break;
    } // DONE

    case 0x6c:{ // LD L, H
        reg[L] = reg[H];
        break;
    } // DONE

    case 0x6d:{ // LD L, L
        reg[L] = reg[L];
        break;
    } // DONE

    case 0x6e:{ // LD L, (HL)
        reg[L] = mem[pack(reg[H], reg[L])];
        break;
    } // DONE

    case 0x6f:{ // LD L, A
        reg[L] = reg[A];
        break;
    } // DONE

    case 0x70:{ // LD (HL), B
        mem[pack(reg[H], reg[L])] = reg[B];
        break;
    } // DONE

    case 0x71:{ // LD (HL), C
        mem[pack(reg[H], reg[L])] = reg[C];
        break;
    } // DONE

    case 0x72:{ // LD (HL), D
        mem[pack(reg[H], reg[L])] = reg[D];
        break;
    } // DONE

    case 0x73:{ // LD (HL), E
        mem[pack(reg[H], reg[L])] = reg[E];
        break;
    } // DONE

    case 0x74:{ // LD (HL), H
        mem[pack(reg[H], reg[L])] = reg[H];
        break;
    } // DONE

    case 0x75:{ // LD (HL), L
        mem[pack(reg[H], reg[L])] = reg[L];
        break;
    } // DONE

    case 0x77:{ // LD (HL), A
        mem[pack(reg[H], reg[L])] = reg[A];
        break;
    } // DONE

    case 0x78:{ // LD A, B
        reg[A] = reg[B];
        break;
    } // DONE

    case 0x79:{ // LD A, C
        reg[A] = reg[C];
        break;
    } // DONE

    case 0x7a:{ // LD A, D
        reg[A] = reg[D];
        break;
    } // DONE

    case 0x7b:{ // LD A, E
        reg[A] = reg[E];
        break;
    } // DONE

    case 0x7c:{ // LD A, H
        reg[A] = reg[H];
        break;
    } // DONE

    case 0x7d:{ // LD A, L
        reg[A] = reg[L];
        break;
    } // DONE

    case 0x7e:{ // LD A, (HL)
        reg[A] = mem[pack(reg[H], reg[L])];
        break;
    } // DONE

    case 0x7f:{ // LD A, A
        reg[A] = reg[A]; // kek
        break;
    } // DONE

    case 0xa0:{ // A = A AND B
        reg[A] = z80_and(reg[A], reg[B]);
        break;
    } // DONE

    case 0xa1:{ // A = A AND C
        reg[A] = z80_and(reg[A], reg[C]);
        break;
    } // DONE

    case 0xa2:{ // A = A AND D
        reg[A] = z80_and(reg[A], reg[D]);
        break;
    } // DONE

    case 0xa3:{ // A = A AND E
        reg[A] = z80_and(reg[A], reg[E]);
        break;
    } // DONE

    case 0xa4:{ // A = A AND H
        reg[A] = z80_and(reg[A], reg[H]);
        break;
    } // DONE

    case 0xa5:{ // A = A AND L
        reg[A] = z80_and(reg[A], reg[L]);
        break;
    } // DONE

    case 0xa6:{ // A = A AND (HL)
        reg[A] = z80_and(reg[A], mem[pack(reg[H], reg[L])]);
        break;
    } // DONE

    case 0xa7:{ // A = A AND A
        reg[A] = z80_and(reg[A], reg[A]);
        break;
    } // DONE

    case 0xa8:{ // A = A XOR B
        reg[A] = z80_xor(reg[A], reg[B]);
        break;
    } // DONE

    case 0xa9:{ // A = A XOR C
        reg[A] = z80_xor(reg[A], reg[C]);
        break;
    } // DONE

    case 0xaa:{ // A = A XOR D
        reg[A] = z80_xor(reg[A], reg[D]);
        break;
    } // DONE

    case 0xab:{ // A = A XOR E
        reg[A] = z80_xor(reg[A], reg[E]);
        break;
    } // DONE

    case 0xac:{ // A = A XOR H
        reg[A] = z80_xor(reg[A], reg[H]);
        break;
    } // DONE

    case 0xad:{ // A = A XOR L
        reg[A] = z80_xor(reg[A], reg[L]);
        break;
    } // DONE

    case 0xae:{ // A = A XOR (HL)
        reg[A] = z80_xor(reg[A], mem[pack(reg[H], reg[L])]);
        break;
    } // DONE

    case 0xaf:{ // A = A XOR A
        reg[A] = z80_xor(reg[A], reg[A]);
        break;
    } // DONE

    case 0xb0:{ // A = A OR B
        reg[A] = z80_or(reg[A], reg[B]);
        break;
    } // DONE

    case 0xb1:{ // A = A OR C
        reg[A] = z80_or(reg[A], reg[C]);
        break;
    } // DONE

    case 0xb2:{ // A = A OR D
        reg[A] = z80_or(reg[A], reg[D]);
        break;
    } // DONE

    case 0xb3:{ // A = A OR E
        reg[A] = z80_or(reg[A], reg[E]);
        break;
    } // DONE

    case 0xb4:{ // A = A OR H
        reg[A] = z80_or(reg[A], reg[H]);
        break;
    } // DONE

    case 0xb5:{ // A = A OR L
        reg[A] = z80_or(reg[A], reg[L]);
        break;
    } // DONE

    case 0xb6:{ // A = A OR (HL)
        reg[A] = z80_or(reg[A], mem[pack(reg[H], reg[L])]);
        break;
    } // DONE

    case 0xb7:{ // A = A OR A
        reg[A] = z80_or(reg[A], reg[A]);
        break;
    } // DONE

    case 0xb8:{ // CP A, B
        z80_cp(reg[A], reg[B]);
        break;
    } // DONE

    case 0xb9:{ // CP A, C
        z80_cp(reg[A], reg[C]);
        break;
    } // DONE

    case 0xba:{ // CP A, D
        z80_cp(reg[A], reg[D]);
        break;
    } // DONE

    case 0xbb:{ // CP A, E
        z80_cp(reg[A], reg[E]);
        break;
    } // DONE

    case 0xbc:{ // CP A, H
        z80_cp(reg[A], reg[H]);
        break;
    } // DONE

    case 0xbd:{ // CP A, L
        z80_cp(reg[A], reg[L]);
        break;
    } // DONE

    case 0xbe:{ // CP A, (HL)
        z80_cp(reg[A], mem[pack(reg[H], reg[L])]);
        break;
    } // DONE

    case 0xbf:{ // CP A, A
        z80_cp(reg[A], reg[A]);
        break;
    } // DONE

    case 0xc3:{ // JP nn
        pc = ((((unsigned short)mem[pc + 2]) << 8) | mem[pc + 1]) - 1;
        break;
    } // DONE

    case 0xe0:{ // LD (0xff00 + n), A
        mem[0xff00 + mem[++pc]] = reg[A];
        break;
    } // DONE

    case 0xe2:{ // LD (0xff00 + C), A
        mem[0xff00 + reg[C]] = reg[A];
        break;
    } // DONE

    case 0xe6:{ // A = A AND n
        reg[A] = z80_and(reg[A], mem[++pc]);
        break;
    } // DONE

    case 0xea:{ // LD (nn), A
        mem[pack(mem[pc + 1], mem[pc + 2])] = reg[A];
        pc += 2;
        break;
    } // DONE

    case 0xee:{ // A = A XOR n
        reg[A] ^= mem[++pc];
        reg[F] = reg[A] ? Z_FLAG : 0x00;
        break;
    } // DONE

    case 0xf0:{ // LD A, (0xff00 + n)
        reg[A] = mem[0xff00 + mem[++pc]];
        break;
    } // DONE

    case 0xf2:{ // LD A, (0xff00 + C)
        reg[A] = mem[0xff00 + reg[C]];
        break;
    } // DONE

    case 0xf6:{ // A = A OR n
        reg[A] = z80_or(reg[A], mem[++pc]);
        break;
    } // DONE

    case 0xfa:{ // LD A, (nn)
        reg[A] = mem[pack(mem[pc + 1], mem[pc + 2])];
        pc += 2;
        break;
    } // DONE

    case 0xfe:{ // CP A, n
        z80_cp(reg[A], mem[++pc]);
        break;
    } // DONE

    default:
        unimpl = opcode; // to signal unimplemented instructions
        break;
    }

    lastop = opcode;
}
