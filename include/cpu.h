#include "alu.h"
#ifndef CPU_DEF
#define CPU_DEF

/*
    The CPU component handles instructions
    Jumps, copying etc. is handled directly here
    Arithmetics are passed on to the ALU object
*/

namespace component{
    class cpu {
    private:
        unsigned char* memory;
        unsigned char* registers;
        bool* F;
        unsigned short pc;

        component::alu* alu;
        void execute(int opcode);
    public:
        unsigned short getPC();
        void step();
        cpu(unsigned char* memory, unsigned char* registers, bool* flags);
    };
}

#endif
