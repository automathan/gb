#ifndef ALU_DEF
#define ALU_DEF

namespace component{
    class alu {
    private:
        char* memory;
        char* registers;
        bool* F;

        void add_reg_to_A(int r, bool adc);
        void and_reg_to_A(int r);
        void or_reg_to_A(int r);
        void xor_reg_to_A(int r);
        void inc_reg(int r);
    public:
        void execute(int opcode);
        alu(char* memory,char* registers, bool* flags);
    };
}

#endif
