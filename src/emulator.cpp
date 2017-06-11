#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include "../include/emulator.h"
#include "../include/alu.h"

enum flags {Z_FLAG, N_FLAG, H_FLAG, C_FLAG};
enum reg_index {A, B, C, D, E, H, L};
char* registers = new char[7];
bool* F = new bool[4];

struct instruction {
        std::string desc = ""; // textual description of the operation
        int op = 0; // number of operand bytes
        void* f;
};

instruction* ops = new instruction[256];
unsigned short pc = 0x100;
char ROM[0x8000];
/*
   void a_op_b(reg_index a, operators op, reg_index b){ // perform A op B and store in A

   }
 */
void mv_r2r(reg_index from, reg_index to){
        registers[to] = registers[from];
}
void mv_v2r_16bit(reg_index to1, reg_index to2, int val1, int val2){
        registers[to1] = val1;
        registers[to2] = val2;
}

int main (){
        registers[A] = 0x01;
        registers[B] = 0x00;
        registers[C] = 0x13;
        registers[D] = 0x00;
        registers[E] = 0xd8;
        registers[H] = 0x01;
        registers[L] = 0x4d;

        std::ifstream ifs ("tetris.gb", std::ifstream::binary);
        if(ifs) {
                // get length of file:
                ifs.seekg (0, ifs.end);
                int length = ifs.tellg();
                ifs.seekg (0, ifs.beg);

                std::cout << "Reading " << length << " characters... ";
                // read data as a block:
                ifs.read(ROM, length);

                if(ifs)
                        std::cout << "all characters read successfully." << std::endl;
                else
                        std::cout << "error: only " << ifs.gcount() << " could be read" << std::endl;
                ifs.close();

                // ...buffer contains the entire file...
                //for(int i = 0x103; i < 0x133; ++i)
                //        std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (+buffer[i] & 0xff) << std::endl;
                std::cout << "ROM name: ";
                for(int i = 0; i < 16; ++i)
                        if(ROM[0x134 + i])
                                std::cout << ROM[0x134 + i];
                std::cout << std::endl;

                for(int i = 0; i < 100; ++i) {
                        bool jmp = 0;
                        int pc_cpy = pc;
                        int opcode = (+ROM[pc] & 0xff);
                        instruction ins = ops[opcode];
                        if(ins.desc.compare("")) {
                                std::cout << std::hex << pc << ": " << ins.desc << " (opcode " << std::hex << opcode << ")"<< std::endl;
                                if(ins.op) {
                                        for(int j = 0; j < ins.op; ++j)
                                                std::cout << "operand #" << (j + 1) << ":  " << (+ROM[pc + j + 1] & 0xff) << std::endl;
                                        pc += ins.op;
                                }
                                if(opcode == 0xc3) {
                                        int addr = ((+ROM[pc_cpy + 2] & 0xff) << 8) | (+ROM[pc_cpy + 1] & 0xff);
                                        pc = addr;
                                        jmp = 1;
                                        std::cout << std::hex << "[jumping to " << addr << "]" << std::endl;
                                }
                                if(opcode == 0x21) {

                                }
                        }else if (opcode) {
                                std::cout << std::endl << "unknown instruction with opcode " << std::hex << opcode  << std::endl;
                        }
                        if(!jmp)
                                pc++;
                }
        }
        return 0;
}

void operation_switch(int opcode){
        if((opcode >= 0x80 && opcode <= 0x85) || opcode == 0x87)
                add_reg_to_A(((opcode % 10) + 1) % 8, false);
        if((opcode >= 0x88 && opcode <= 0x8D) || opcode == 0x8F)
                add_reg_to_A(((opcode & 0xf) - 7) % 8, true);
        if((opcode >= 0xA0 && opcode <= 0xA5) || opcode == 0xA7)
                and_reg_to_A(((opcode % 10) + 1) % 8);
        if((opcode >= 0xB0 && opcode <= 0xB5) || opcode == 0xB7)
                or_reg_to_A(((opcode % 10) + 1) % 8);
        if((opcode >= 0xA8 && opcode <= 0xAD) || opcode == 0xAF)
                xor_reg_to_A(((opcode & 0xf) - 7) % 8);
        if((opcode >= 0x04 && opcode <= 0x2C && ((opcode + 4) % 8 == 0)) || opcode == 0x3C)
                inc_reg(((opcode + 4) >> 2) % 8);
}
