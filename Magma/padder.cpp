#include "padder.h"

uint64_t Padder::procedure2(uint64_t P, size_t r, size_t l) {
    return (P << (l - r)) | ((uint64_t)0x1 << (l - r - 1));
}

uint64_t Padder::procedure3(uint64_t P, size_t r, size_t l) {
    if (r == 64)
        return P;
    else
        return procedure2(P, r, l);
}