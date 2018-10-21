#include "basic_block_cypher.h"

const char BasicBlockCypher::pi[128] = {
    // pi 0
    12, 4, 6, 2, 10, 5, 11, 9, 14, 8, 13, 7, 0, 3, 15, 1,
    // pi 1
    6, 8, 2, 3, 9, 10, 5, 12, 1, 14, 4, 7, 11, 13, 0, 15,
    // pi 2
    11, 3, 5, 8, 2, 15, 10, 13, 14, 1, 7, 4, 12, 9, 6, 0,
    // pi 3
    12, 8, 2, 1, 13, 4, 15, 6, 7, 0, 10, 5, 3, 14, 9, 11,
    // pi 4
    7, 15, 5, 10, 8, 1, 6, 13, 0, 9, 3, 14, 11, 4, 2, 12,
    // pi 5
    5, 13, 15, 6, 9, 2, 12, 10, 11, 7, 8, 1, 4, 3, 14, 0,
    // pi 6
    8, 14, 2, 5, 6, 9, 1, 12, 15, 4, 11, 0, 13, 10, 3, 7,
    // pi 7
    1, 7, 14, 13, 0, 5, 8, 3, 4, 15, 10, 6, 9, 12, 11, 2};

uint32_t BasicBlockCypher::t(uint32_t a) {
    uint32_t mask = 0xf, res = 0, val = 0;
    for (size_t i = 0; i < 8; i++) {
        val = (a & mask) >> i * 4;
        res |= pi[i * 16 + val] << i * 4;
        mask <<= 4;
    }
    return res;
}

uint32_t BasicBlockCypher::g(uint32_t k, uint32_t a) {
    uint32_t c = t(k + a);
    return (c << 11) | (c >> 21);
}

pair<uint32_t, uint32_t> BasicBlockCypher::G(uint32_t k, uint32_t a1,
                                             uint32_t a0) {
    return pair<uint32_t, uint32_t>(a0, g(k, a0) ^ a1);
}

uint64_t BasicBlockCypher::G1(uint32_t k, uint32_t a1, uint32_t a0) {
    return ((uint64_t)g(k, a0) ^ a1) << 32 | a0;
}

void BasicBlockCypher::key_schedule(uint32_t K[8]) {
    for (int i = 0; i < 8; i++) this->K[i] = K[i];
    for (int i = 0; i < 8; i++) this->K[i + 8] = K[i];
    for (int i = 0; i < 8; i++) this->K[i + 16] = K[i];
    for (int i = 0; i < 8; i++) this->K[i + 24] = K[7 - i];
}

uint64_t BasicBlockCypher::e(uint64_t P) {
    uint32_t a0 = P & 0xffffffff, a1 = P >> 32;
    pair<uint32_t, uint32_t> a(a1, a0);
    for (int i = 0; i < 31; i++) a = G(K[i], a.first, a.second);
    return G1(K[31], a.first, a.second);
}

uint64_t BasicBlockCypher::d(uint64_t P){
    uint32_t a0 = P & 0xffffffff, a1 = P >> 32;
    pair<uint32_t, uint32_t> a(a1, a0);
    for (int i = 31; i > 0; i--) a = G(K[i], a.first, a.second);
    return G1(K[0], a.first, a.second);
}
