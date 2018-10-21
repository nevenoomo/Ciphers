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
};