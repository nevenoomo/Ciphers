#include "modes.h"

void ECB::encrypt() {  // TODO test
    size_t bytes_read = 0;
    bool padded = false;
    uint8_t P[8] = {0};
    uint8_t *C = 0;
    while (!feof(input)) {
        bytes_read = fread(P, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read == 0) break;
        if (bytes_read < 8) padded = (bool)Utils::padd_proc2(P, bytes_read, 8);
        C = Utils::convert_to_arr(b.e(Utils::convert_to_num(P)));
        fwrite(C, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] C;
    }
    if (!padded) {
        uint8_t P1[8] = {0};
        Utils::padd_proc2(P1, 0, 8);
        C = Utils::convert_to_arr(b.e(Utils::convert_to_num(P1)));
        fwrite(C, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] C;
    }
}

void ECB::decrypt() {  // TODO test, check len
    size_t bytes_read = 0;
    int c = 0;
    uint8_t C[8] = {0};
    uint8_t *P = 0;
    while (!feof(input)) {
        bytes_read = fread(C, sizeof(uint8_t), BLOCK_SIZE, input);
        if ((c = fgetc(input)) == EOF) {
            P = Utils::convert_to_arr(b.d(Utils::convert_to_num(C)));
            for (c = BLOCK_SIZE; c >= 0 && C[c] == 0x0; c--);
            if (c == -1 || C[c] != 0xf0)
                fwrite(P, sizeof(uint8_t), BLOCK_SIZE, output);
            else if (C[c] == 0xf0)
                fwrite(P, sizeof(uint8_t), c, output);
            delete[] P;
            break;
        } else {
            ungetc(c, input);
        }
        P = Utils::convert_to_arr(b.d(Utils::convert_to_num(C)));
        fwrite(P, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] P;
    }
}

void CTR::encrypt() {
    uint64_t P = 0, C = 0;
    unsigned char P1[8] = {0};
    uint8_t *C1 = 0;
    size_t bytes_read = 0;
    while (!feof(input)) {
        bytes_read = fread(P1, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read == 0) break;
        P = Utils::convert_to_num(P1);
        C = (P >> (8 - bytes_read) * 8) ^ Utils::T(bytes_read, b.e(this->c));
        C1 = Utils::convert_to_arr(C);
        fwrite(C1, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] C1;
        Add();
    }
}

void CTR::decrypt() {
    uint64_t P = 0, C = 0;
    unsigned char C1[8] = {0};
    uint8_t *P1 = 0;
    size_t bytes_read = 0;
    while (!feof(input)) {
        bytes_read = fread(C1, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read == 0) break;
        C = Utils::convert_to_num(C1);
        P = (C >> (8 - bytes_read) * 8) ^ Utils::T(bytes_read, b.e(this->c));
        P1 = Utils::convert_to_arr(P);
        fwrite(P1, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] P1;
        Add();
    }
}

void OFB::encrypt() {
    uint64_t P = 0, C = 0, Y = 0;
    unsigned char P1[8] = {0};
    uint8_t *C1 = 0;
    size_t bytes_read = 0;
    while (!feof(input)) {
        bytes_read = fread(P1, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read == 0) break;
        P = Utils::convert_to_num(P1);
        Y = b.e(R1);
        C = (P >> (8 - bytes_read) * 8) ^ Utils::T(bytes_read, Y);
        Shift(Y);
        C1 = Utils::convert_to_arr(C);
        fwrite(C1, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] C1;
    }
}

void OFB::decrypt() {
    uint64_t P = 0, C = 0, Y = 0;
    unsigned char C1[8] = {0};
    uint8_t *P1 = 0;
    size_t bytes_read = 0;
    while (!feof(input)) {
        bytes_read = fread(C1, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read == 0) break;
        C = Utils::convert_to_num(C1);
        Y = b.e(R1);
        P = (C >> (8 - bytes_read) * 8) ^ Utils::T(bytes_read, Y);
        Shift(Y);
        P1 = Utils::convert_to_arr(P);
        fwrite(P1, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] P1;
    }
}