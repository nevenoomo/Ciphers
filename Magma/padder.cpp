#include "padder.h"

uint8_t* Padder::convert_to_arr(uint64_t P) {
    uint8_t* P1 = new uint8_t[8];
    uint64_t mask = 0xff00000000000000;
    for (int i = 0; i < 8; i++) {
        P1[i] = (P & mask) >> ((7 - i) * 8);
        mask >>= 8;
    }
    return P1;
}

uint64_t Padder::convert_to_num(uint8_t* P) {
    uint64_t res = 0;
    for (int i = 0; i < 8; i++) {
        res |= (uint64_t)P[i] << ((7 - i) * 8);
    }
    return res;
}

uint64_t Padder::procedure2(uint64_t P, size_t r, size_t l) {
    uint8_t* P1 = convert_to_arr(P);
    P1[r] = 0x80;
    for (int i = r + 1; i < l; i++) P1[i] = 0x0;
    uint64_t res = convert_to_num(P1);
    delete[] P1;
    return res;
}

uint64_t Padder::procedure3(uint64_t P, size_t r, size_t l) {
    if (r == 8)
        return P;
    else
        return procedure2(P, r, l);
}