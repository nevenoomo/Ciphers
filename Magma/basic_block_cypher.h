#pragma once
#include "common.h"

class BasicBlockCypher {
    static const char pi[128];
    vector<uint32_t> K;

    uint32_t t(uint32_t a);
    uint32_t g(uint32_t k, uint32_t a);
    pair<uint32_t, uint32_t> G(uint32_t k, uint32_t a1, uint32_t a0);
    uint64_t G1(uint32_t k, uint32_t a1, uint32_t a0);
    void key_schedule(uint32_t K[8]);
   public:
    uint64_t e(uint64_t P);
    uint64_t d(uint64_t P);

    BasicBlockCypher(uint32_t K[8]): K(32){
        key_schedule(K);
    }
};