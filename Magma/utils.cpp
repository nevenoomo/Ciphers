#include "utils.h"

uint64_t Utils::T(size_t s, uint64_t P){
    uint64_t mask = 0;
    for (size_t i = 0; i < s; i++) mask |= 0xff00000000000000>>(i*8);
    return (P&mask)>>((8-s)*8);
}

uint64_t Utils::filesize(istream & s){
    s.seekg(0, ios_base::end);
    uint64_t size = s.tellg();
    s.seekg(0, ios_base::beg);
    return size;
}

uint8_t* Utils::convert_to_arr(uint64_t P) {
    uint8_t* P1 = new uint8_t[8];
    uint64_t mask = 0xff00000000000000;
    for (int i = 0; i < 8; i++) {
        P1[i] = (P & mask) >> ((7 - i) * 8);
        mask >>= 8;
    }
    return P1;
}

uint64_t Utils::convert_to_num(uint8_t* P) {
    uint64_t res = 0;
    for (int i = 0; i < 8; i++) {
        res |= (uint64_t)P[i] << ((7 - i) * 8);
    }
    return res;
}

uint64_t Utils::padd_proc2(uint8_t* P, size_t r, size_t l) {
    P[r] = 0x80;
    for (size_t i = r + 1; i < l; i++) P[i] = 0x0;
    return convert_to_num(P);
}

uint64_t Utils::padd_proc2(uint64_t P, size_t r, size_t l) {
    uint8_t* P1 = convert_to_arr(P);
    uint64_t res = padd_proc2(P1, r, l);
    delete[] P1;
    return res;
}

uint64_t Utils::padd_proc3(uint8_t* P, size_t r, size_t l){
    if (r == 8)
        return convert_to_num(P);
    else
        return padd_proc2(P, r, l);
}


uint64_t Utils::padd_proc3(uint64_t P, size_t r, size_t l) {
    if (r == 8)
        return P;
    else
        return padd_proc2(P, r, l);
}