#pragma once
#include "basic_block_cypher.h"
#include "utils.h"

class ECB {
    BasicBlockCypher b;
    istream& input;
    ostream& output;

   public:
    ECB(uint32_t K[8], istream& i, ostream& o) : b(K), input(i), output(o) {}
    void encrypt();
    void decrypt();
};

class CTR {
    BasicBlockCypher b;
    uint32_t IV;
    istream& input;
    ostream& output;
    uint64_t c;

    void Add() { c++; }

   public:
    CTR(uint32_t K[8], uint32_t IV, istream& i, ostream& o)
        : b(K), IV(IV), input(i), output(o), c((uint64_t)IV << 32) {}
    void encrypt();
    void decrypt();
};