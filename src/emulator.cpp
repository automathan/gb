#include "../include/emulator.h"

typedef unsigned char byte;
enum flags {Z_FLAG, N_FLAG, H_FLAG, C_FLAG};
enum reg_index {A, B, C, D, E, H, L, F};
byte* registers = new byte[8];

emulator::ROM* emulator::loadROM(const char* path){
    std::ifstream ifs (path, std::ifstream::binary);
    byte* cartridge;
	if(ifs) {
		ifs.seekg (0, ifs.end);
		int length = ifs.tellg();
		ifs.seekg (0, ifs.beg);
        cartridge = (byte*)malloc(length);

        ifs.read((char*)cartridge, length);

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

    // 0x0000 - 0x7fff = ROM
    // 0x8000 - 0xffff = internal memory

    memory = (byte*)malloc(0x10000);
    registers = new byte[8];

	// initial register values
	registers[A] = 0x01;
	registers[B] = 0x00;
	registers[C] = 0x13;
	registers[D] = 0x00;
	registers[E] = 0xd8;
	registers[H] = 0x01;
	registers[L] = 0x4d;
    registers[F] = 0x00; // FLAGS

	ROM* rom = loadROM(path);

	if(rom){
        emit debug("successfully loaded ROM");
        emit debug("ROM size = 0x" + QString("%1").arg(rom->size, 4, 16, QChar('0')) + " bytes");
        emit debug("NAME name = " + QString(rom->name.c_str()));
        memcpy(memory, rom->cartridge, 0x8000);
    }else{
        emit debug("failed to load ROM");
    }

    cpu = new component::cpu(memory, registers);
    gpu = new component::gpu(memory);
}

byte* emulator::getRegs(){
    return registers;
}

unsigned short emulator::getPC(){
    return cpu->getPC();
}

unsigned short emulator::getSP(){
    return cpu->getSP();
}


void emulator::step(){
    auto pc = cpu->getPC();
    cpu->step();
    if(cpu->unimpl != -1)
        emit debug("0x" + QString("%1").arg(pc, 4, 16, QChar('0')) + ": unimplemented operation: " + QString::number(cpu->unimpl,16));
    else
        emit debug("0x" + QString("%1").arg(pc, 4, 16, QChar('0')) + ": valid operation: " + QString::number(cpu->lastop,16));
}

std::vector<byte> emulator::getFrame(int step){
    emit debug("visualizing ROM with offset " + QString::number(gpu->getTO()));
    return gpu->getFrame(step);
}
