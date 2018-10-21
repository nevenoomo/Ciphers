#include "modes.h"

void ECB::encrypt() {
    size_t i = 0;
    unsigned char P1[8] = {0};
    uint8_t *P2 = 0;
    while (!input.eof()) {
        input.read((char *)P1, 8 * sizeof(char));
        if ((i = input.gcount()) < 8) Utils::padd_proc2(P1, i, 8);
        P2 = Utils::convert_to_arr(b.e(Utils::convert_to_num(P1)));
        output.write((char *)P2, sizeof(uint64_t));
        delete[] P2;
        if (input.peek() == EOF) break;
    }
}

void ECB::decrypt() {
    size_t i = 0;
    unsigned char P1[8] = {0};
    uint8_t *P2 = 0;
    while (!input.eof()) {
        input.read((char *)P1, 8 * sizeof(char));
        if ((i = input.gcount()) < 8) Utils::padd_proc2(P1, i, 8);
        P2 = Utils::convert_to_arr(b.d(Utils::convert_to_num(P1)));
        output.write((char *)P2, sizeof(uint64_t));
        delete[] P2;
        if (input.peek() == EOF) break;
    }
}

void CTR::encrypt() {
    uint64_t P = 0, C = 0;
    unsigned char P1[8] = {0};
    uint8_t *C1 = 0;
    size_t r = 0;
    while (!input.eof()) {
        input.read((char *)P1, 8 * sizeof(char));
        r = input.gcount();
        P = Utils::convert_to_num(P1);
        C = (P>>(8-r)*8) ^ Utils::T(r, b.e(this->c));
        C1 = Utils::convert_to_arr(C);
        output.write((char *)C1, sizeof(uint64_t));
        delete[] C1;
        Add();
        if (input.peek() == EOF) break;
    }
}

void CTR::decrypt() {
    uint64_t P = 0, C = 0;
    unsigned char C1[8] = {0};
    uint8_t *P1 = 0;
    size_t r = 0;
    while (!input.eof()) {
        input.read((char *)C1, 8 * sizeof(char));
        r = input.gcount();
        C = Utils::convert_to_num(C1);
        P = (C>>(8-r)*8) ^ Utils::T(r, b.e(this->c));
        P1 = Utils::convert_to_arr(P);
        output.write((char *)P1, sizeof(uint64_t));
        delete[] P1;
        Add();
        if (input.peek() == EOF) break;
    }
}