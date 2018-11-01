#include "modes.h"

void ECB::encrypt() {
    size_t bytes_read = 0;
    uint8_t P[8] = {0};
    uint8_t *C = 0;
    while (!feof(input)) {
        bytes_read = fread(P, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read < 8) Utils::padd_proc2(P, bytes_read, 8);
        C = Utils::convert_to_arr(b.e(Utils::convert_to_num(P)));
        fwrite(C, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] C;
    }
}

void ECB::decrypt() {
    size_t bytes_read = 0;
    int c = 0;
    uint8_t C[8] = {0};
    uint8_t *P = 0;
    while (!feof(input)) {
        bytes_read = fread(C, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read < 8)
            ;  // TODO error
        P = Utils::convert_to_arr(b.d(Utils::convert_to_num(C)));
        if ((c = fgetc(input)) == EOF) {
            for (c = BLOCK_SIZE - 1; c >= 0 && P[c] == 0x0; c--)
                ;
            if (c == -1 || P[c] != 0x80)
                fwrite(P, sizeof(uint8_t), BLOCK_SIZE, output);
            else if (P[c] == 0x80)
                fwrite(P, sizeof(uint8_t), c, output);
            delete[] P;
            break;
        } else {
            ungetc(c, input);
        }
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
        C = ((P >> (8 - bytes_read) * 8) ^ Utils::T(bytes_read, b.e(this->c)))
            << (8 - bytes_read) * 8;
        C1 = Utils::convert_to_arr(C);
        fwrite(C1, sizeof(uint8_t), bytes_read, output);
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
        P = ((C >> (8 - bytes_read) * 8) ^ Utils::T(bytes_read, b.e(this->c)))
            << (8 - bytes_read) * 8;
        P1 = Utils::convert_to_arr(P);
        fwrite(P1, sizeof(uint8_t), bytes_read, output);
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
        Y = b.e(R[0]);
        C = ((P >> (8 - bytes_read) * 8) ^ Utils::T(bytes_read, Y))
            << (8 - bytes_read) * 8;
        Shift(Y);
        C1 = Utils::convert_to_arr(C);
        fwrite(C1, sizeof(uint8_t), bytes_read, output);
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
        Y = b.e(R[0]);
        P = ((C >> (8 - bytes_read) * 8) ^ Utils::T(bytes_read, Y))
            << (8 - bytes_read) * 8;
        Shift(Y);
        P1 = Utils::convert_to_arr(P);
        fwrite(P1, sizeof(uint8_t), bytes_read, output);
        delete[] P1;
    }
}

void CBC::encrypt() {
    uint64_t C = 0;
    unsigned char P[8] = {0};
    uint8_t *C1 = 0;
    size_t bytes_read = 0;
    while (!feof(input)) {
        bytes_read = fread(P, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read < BLOCK_SIZE) Utils::padd_proc2(P, bytes_read, 8);
        C = b.e(Utils::convert_to_num(P) ^ R[0]);
        Shift(C);
        C1 = Utils::convert_to_arr(C);
        fwrite(C1, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] C1;
    }
}

void CBC::decrypt() {
    uint64_t C = 0;
    int c = 0;
    unsigned char C1[8] = {0};
    uint8_t *P1 = 0;
    size_t bytes_read = 0;
    while (!feof(input)) {
        bytes_read = fread(C1, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read < 8)
            ;  // TODO error
        C = Utils::convert_to_num(C1);
        P1 = Utils::convert_to_arr(b.d(C) ^ R[0]);
        if ((c = fgetc(input)) == EOF) {
            for (c = BLOCK_SIZE - 1; c >= 0 && P1[c] == 0x0; c--){}
            if (c == -1 || P1[c] != 0x80)
                ;  // TODO error
            else if (P1[c] == 0x80)
                fwrite(P1, sizeof(uint8_t), c, output);
            delete[] P1;
            break;
        } else {
            ungetc(c, input);
        }
        Shift(C);
        fwrite(P1, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] P1;
    }
}

void CFB::encrypt() {
    size_t bytes_read = 0;
    uint64_t C = 0, P = 0;
    uint8_t P1[8] = {0};
    uint8_t *C1 = 0;
    while (!feof(input)) {
        bytes_read = fread(P1, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read < 8) Utils::padd_proc2(P1, bytes_read, 8);
        P = Utils::convert_to_num(P1);
        C = P ^ b.e(R[0]);
        Shift(C);
        C1 = Utils::convert_to_arr(C);
        fwrite(C1, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] C1;
    }
}

void CFB::decrypt() {
    size_t bytes_read = 0;
    int c = 0;
    uint64_t C = 0, P = 0;
    uint8_t C1[8] = {0};
    uint8_t *P1 = 0;
    while (!feof(input)) {
        bytes_read = fread(C1, sizeof(uint8_t), BLOCK_SIZE, input);
        if (bytes_read < 8)
            ;  // TODO error
        C = Utils::convert_to_num(C1);
        P = C ^ b.e(R[0]);
        P1 = Utils::convert_to_arr(P);
        Shift(C);
        if ((c = fgetc(input)) == EOF) {
            for (c = BLOCK_SIZE - 1; c >= 0 && P1[c] == 0x0; c--) {}
            if (c == -1 || P1[c] != 0x80)
                ;  // TODO error
            else if (P1[c] == 0x80)
                fwrite(P1, sizeof(uint8_t), c, output);
            delete[] P1;
            break;
        } else {
            ungetc(c, input);
        }
        fwrite(P1, sizeof(uint8_t), BLOCK_SIZE, output);
        delete[] P1;
    }
}