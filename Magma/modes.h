#pragma once
#include <algorithm>
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
    vector<uint64_t> R;
    FILE* input;
    FILE* output;

    void Shift(uint64_t Y) {
        std::copy(R.begin() + 1, R.end(), R.begin());
        R.pop_back();
        R.push_back(Y);
    }

   public:
    OFB(uint32_t K[8], vector<uint64_t> IV, FILE* i, FILE* o)
        : b(K), R(IV), input(i), output(o) {}
    void encrypt();
    void decrypt();
};

class CBC {
    // m = z*n
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
    CBC(uint32_t K[8], vector<uint64_t> IV, FILE* i, FILE* o)
        : b(K), R(IV), input(i), output(o) {}
    void encrypt();
    void decrypt();
};

class CFB {
    // m = z*n s = n = 64
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
    CFB(uint32_t K[8], vector<uint64_t> IV, FILE* i, FILE* o)
        : b(K), R(IV), input(i), output(o) {}
    void encrypt();
    void decrypt();
};

class MAC {
    // s = 32
    BasicBlockCypher b;
    uint64_t R;
    FILE* input;
    FILE* output;
    const uint64_t B64 = 0x1b;  // 0^59||11011
    uint64_t K1;
    uint64_t K2;

   public:
    MAC(uint32_t K[8], FILE *i, FILE *o);

    void generate();
};