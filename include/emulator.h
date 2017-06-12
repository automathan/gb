#include "alu.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

class emulator{
private:
    char* memory;
    char* registers;
    bool* F;

    struct ROM{
    	std::string name;
    	int size;
    	char* cartridge;
    };

    component::alu* alu;
    ROM* loadROM(const char*); // TODO move to romloader
public:
    //emulator();
    void play(const char*);
};
