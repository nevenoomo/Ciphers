#pragma once
#include "basic_block_cypher.h"
#include "utils.h"

class ECB {
    BasicBlockCypher b;
    FILE* input;
    FILE* output;

   public:
    ECB(uint32_t K[8], FILE* i, FILE* o) : b(K), input(i), output(o) {}
    void encrypt();
    void decrypt();
};

class CTR {
    BasicBlockCypher b;
    FILE* input;
    FILE* output;
    uint64_t c;

    void Add() { c++; }

   public:
    CTR(uint32_t K[8], uint32_t IV, FILE* i, FILE* o)
        : b(K), input(i), output(o), c((uint64_t)IV << 32) {}
    void encrypt();
    void decrypt();
};

class OFB {
    BasicBlockCypher b;
    uint64_t R1;
    uint64_t R0;
    FILE* input;
    FILE* output;

    void Shift(uint64_t Y) {
        R1 = R0;
        R0 = Y;
    }

   public:
    // IV = IV1||IV0
    OFB(uint32_t K[8], uint64_t IV1, uint64_t IV0, FILE* i, FILE* o)
        : b(K), R1(IV1), R0(IV0), input(i), output(o) {}
    void encrypt();
    void decrypt();
};