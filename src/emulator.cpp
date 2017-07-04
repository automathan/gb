#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include "../include/emulator.h"
#include "../include/alu.h"
#include "../include/gpu.h"

enum flags {Z_FLAG, N_FLAG, H_FLAG, C_FLAG};
enum reg_index {A, B, C, D, E, H, L};
char* registers = new char[7];
bool* F = new bool[4];

/*
struct instruction {
	std::string desc = "";                                                                           // textual description of the operation
	int op = 0;                                                                                        // number of operand bytes
	void* f;
};

instruction* ops = new instruction[256];
unsigned short pc = 0x100;
*/
//char ROM[0x8000];
/*
   void a_op_b(reg_index a, operators op, reg_index b){ // perform A op B and store in A

   }
 */
 /*
emulator::emulator(){

}
*/



emulator::ROM* emulator::loadROM(const char* path){ // TODO move to romloader
	std::ifstream ifs (path, std::ifstream::binary);
	char* cartridge;
	if(ifs) {
		ifs.seekg (0, ifs.end);
		int length = ifs.tellg();
		ifs.seekg (0, ifs.beg);
		cartridge = (char*)malloc(length);

		ifs.read(cartridge, length);

		if(!ifs)
			return nullptr;

		ifs.close();

		std::string name = "";
		int i = 0x134;
		while(cartridge[i])
			name += cartridge[i++];

		return new ROM {name, length, cartridge};
    }
	return nullptr;
}

void emulator::play(const char* path){
    emit debug("initializing...");
	// allocate memory
    memory = new unsigned char[0x10000];
    registers = new unsigned char[7];
    F = new bool[4];

	// initial register values
	registers[A] = 0x01;
	registers[B] = 0x00;
	registers[C] = 0x13;
	registers[D] = 0x00;
	registers[E] = 0xd8;
	registers[H] = 0x01;
	registers[L] = 0x4d;

	ROM* rom = loadROM(path);

	if(rom){
        emit debug("successfully loaded ROM");
        emit debug("rom name = " + QString(rom->name.c_str()));
        memcpy(memory, rom->cartridge, 0x8000);
        //std::cout << "ROM loaded!" << std::endl;
        //std::cout << "name: " << rom->name << std::endl;
        //std::cout << "size: 0x" << std::hex << rom->size << std::endl;
    }else{
        emit debug("failed to load ROM");
    }

    cpu = new component::cpu(memory, registers, F);
    gpu = new component::gpu(memory);

}

void emulator::step(){
    auto pc = cpu->getPC();
    cpu->step();
    if(memory[0])
        emit debug(QString::number(pc,16) + ": unimplemented operation: " + QString::number(memory[0],16));
    else
        emit debug(QString::number(pc,16) + ": valid operation: " + QString::number(memory[1],16));
    if(memory[2]){
        if(memory[2] == 1)
            emit debug("param = " + QString::number(memory[3],16));
        if(memory[2] == 2)
            emit debug("params = " + QString::number(memory[3],16) + ", " + QString::number(memory[4],16));
    }
}

std::vector<unsigned char> emulator::getFrame(){
    return gpu->getFrame();
}

/*
int main (){
	//std::ifstream ifs ("tetris.gb", std::ifstream::binary);
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
*/
