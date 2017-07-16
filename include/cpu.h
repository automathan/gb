#ifndef CPU_DEF
#define CPU_DEF

namespace component{
    class cpu {
    private:
        unsigned char* mem;
        unsigned char* reg;
        unsigned short pc; // program counter
        unsigned short sp; // stack pointer

        void execute(int opcode);
        unsigned short pack(unsigned char, unsigned char);
    public:
        unsigned short getPC();
        void step();
        cpu(unsigned char* mem, unsigned char* reg);
    };
}

#endif
