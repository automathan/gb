#include "../include/alu.h"
enum flags {Z_FLAG, N_FLAG, H_FLAG, C_FLAG};
enum reg_index {A, B, C, D, E, H, L};

void add_reg_to_A(int r, bool adc){
        if(registers[A] & 0x8 && registers[r] & 0x8) F[H_FLAG] = 1;
        if(registers[A] & 0x80 && registers[r] & 0x80) F[C_FLAG] = 1;
        F[N_FLAG] = 0;
        registers[A] += registers[r] + (adc ? F[C_FLAG] : 0);
        if(!registers[A]) F[Z_FLAG] = 1;
}

void and_reg_to_A(int r){
        F[N_FLAG] = 0;
        F[H_FLAG] = 1;
        F[C_FLAG] = 0;
        registers[A] &= registers[r];
        if(!registers[A]) F[Z_FLAG] = 1;
}

void or_reg_to_A(int r){
        F[N_FLAG] = 0;
        F[H_FLAG] = 0;
        F[C_FLAG] = 0;
        registers[A] |= registers[r];
        if(!registers[A]) F[Z_FLAG] = 1;
}

void xor_reg_to_A(int r){
        F[N_FLAG] = 0;
        F[H_FLAG] = 0;
        F[C_FLAG] = 0;
        registers[A] ^= registers[r];
        if(!registers[A]) F[Z_FLAG] = 1;
}

void inc_reg(int r){
        if(registers[A] & 0x8 && registers[r] & 0x8) F[H_FLAG] = 1;
        F[N_FLAG] = 0;
        registers[A]++;
        if(!registers[A]) F[Z_FLAG] = 1;
}
