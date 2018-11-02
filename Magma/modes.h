#pragma once
#include <algorithm>
#include "basic_block_cypher.h"
#include "utils.h"
namespace Modes {
struct Mode {
    virtual void run(bool enc) {}
    virtual ~Mode() {}
};

class ECB : public Mode {
    BasicBlockCypher b;
    FILE* input;
    FILE* output;

   public:
    ECB(uint32_t K[8], FILE* i, FILE* o) : b(K), input(i), output(o) {}
    void encrypt();
    void decrypt();
    void run(bool enc) override {
        if (enc)
            encrypt();
        else
            decrypt();
    }
};

class CTR : public Mode {
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
    void run(bool enc) override {
        if (enc)
            encrypt();
        else
            decrypt();
    }
};

class OFB : public Mode {
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
    void run(bool enc) override {
        if (enc)
            encrypt();
        else
            decrypt();
    }
};

class CBC : public Mode {
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
    void run(bool enc) override {
        if (enc)
            encrypt();
        else
            decrypt();
    }
};

class CFB : public Mode {
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
    
    void run(bool enc) override {
        if (enc)
            encrypt();
        else
            decrypt();
    }
};

class MAC : public Mode {
    // s = 32
    BasicBlockCypher b;
    uint64_t R;
    FILE* input;
    FILE* output;
    const uint64_t B64 = 0x1b;  // 0^59||11011
    uint64_t K1;
    uint64_t K2;

   public:
    MAC(uint32_t K[8], FILE* i, FILE* o);

    void generate();
    void run(bool enc) override { generate(); }
};
}  // namespace Modes