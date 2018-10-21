#include "modes.h"

void ECB::encrypt() {
    uint64_t P = 0;
    unsigned char P1[8] = {0};
    uint8_t * P2 = 0;
    while (!input.eof()) {
        input.read((char *)P1, 8*sizeof(char));
        P = Utils::convert_to_num(P1);
        P2 = Utils::convert_to_arr(b.e(P));
        output.write((char *)P2, sizeof(P));
        delete[] P2;
        if (input.peek() == EOF) break;
    }
}