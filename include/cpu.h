#ifndef CPU_DEF
#define CPU_DEF

namespace component{
    class cpu {
    private:
        unsigned char* memory;
        unsigned char* registers;
        unsigned short pc; // program counter
        unsigned short sp; // stack pointer

        void execute(int opcode);
    public:
        unsigned short getPC();
        void step();
        cpu(unsigned char* memory, unsigned char* registers);
    };
}

#endif
