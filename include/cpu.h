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
        char* memory;
        char* registers;
        bool* F;
        unsigned short pc; // internal register

        component::alu* alu;
    public:
        void execute(int opcode);
        cpu(char* memory,char* registers, bool* flags);
    };
}

#endif
