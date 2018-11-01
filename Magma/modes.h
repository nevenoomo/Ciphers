#pragma once
#include "basic_block_cypher.h"
#include "utils.h"
#include <algorithm>

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
    vector<uint64_t> R;
    FILE* input;
    FILE* output;

    void Shift(uint64_t Y) {
        std::copy(R.begin() + 1, R.end(), R.begin());
        R.pop_back();
        R.push_back(Y);
    }

   public:
    // IV = IV1||IV0
    OFB(uint32_t K[8], vector<uint64_t> IV, FILE* i, FILE* o)
        : b(K), R(IV), input(i), output(o) {}
    void encrypt();
    void decrypt();
};

class CBC {
    //m = z*n
    BasicBlockCypher b;
    vector<uint64_t> R;    
    FILE* input;
    FILE* output;

    void Shift(uint64_t Y) {
        std::copy(R.begin() + 1, R.end(),R.begin());
        R.pop_back();
        R.push_back(Y);
    }

   public:
    // IV = IV2||IV1||IV0
    CBC(uint32_t K[8], vector<uint64_t> IV, FILE* i, FILE* o)
        : b(K), R(IV), input(i), output(o) {}
    void encrypt();
    void decrypt();
};

class CFB{
    //m = z*n s = n = 64
    BasicBlockCypher b;
    vector<uint64_t> R;    
    FILE* input;
    FILE* output;

    void Shift(uint64_t Y) {
        std::copy(R.begin() + 1, R.end(),R.begin());
        R.pop_back();
        R.push_back(Y);
    }

   public:
    // IV = IV2||IV1||IV0
    CFB(uint32_t K[8], vector<uint64_t> IV, FILE* i, FILE* o)
        : b(K), R(IV), input(i), output(o) {}
    void encrypt();
    void decrypt();
};