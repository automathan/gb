#ifndef ALU_DEF
#define ALU_DEF

/*
    The ALU component is responsible
    for carrying out aritmetics and logical
    instructions received by the CPU
*/

namespace component{
    class alu {
    private:
        unsigned char* memory;
        unsigned char* registers;
        bool* F;

        void add_reg_to_A(int r, bool adc);
        void and_reg_to_A(int r);
        void or_reg_to_A(int r);
        void xor_reg_to_A(int r);
        void inc_reg(int r);
    public:
        bool execute(int opcode);
        alu(unsigned char* memory,unsigned char* registers, bool* flags);
    };
}

#endif
