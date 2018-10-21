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