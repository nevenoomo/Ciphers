#pragma once
#include "common.h"

class Padder {
    static uint8_t* convert_to_arr(uint64_t P);
    static uint64_t convert_to_num(uint8_t* P);

   public:
    static uint64_t procedure2(uint64_t P, size_t r, size_t l);  //|P| = r mod l
    static uint64_t procedure3(uint64_t P, size_t r, size_t l);  //|P| = r mod l
};