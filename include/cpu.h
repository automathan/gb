#ifndef CPU_DEF
#define CPU_DEF

namespace component{

typedef unsigned char byte;
    class cpu {
    private:
        byte* mem;
        byte* reg;
        unsigned short pc; // program counter
        unsigned short sp; // stack pointer

        // wrapper functions for ops to keep flagging under control
        inline void z80_cp(byte,byte);
        inline byte z80_xor(byte,byte);
        inline byte z80_or(byte,byte);
        inline byte z80_and(byte,byte);

        void execute(int);
        unsigned short pack(byte, byte);
    public:
        unsigned short getPC();
        void step();
        cpu(byte* mem, byte* reg);
    };
}

#endif
