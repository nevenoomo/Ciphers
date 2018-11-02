#pragma once
#include "common.h"

class Utils {
   public:
    static size_t file_size(FILE *);
    static uint64_t T(size_t s, uint64_t P);//s in bytes
    static uint8_t* convert_to_arr(uint64_t P);
    static uint64_t convert_to_num(uint8_t* P);
    static uint32_t convert_to_num32(uint8_t* P);
    static uint64_t padd_proc2(uint8_t* P, size_t r, size_t l);  //|P| = r mod l
    static uint64_t padd_proc2(uint64_t P, size_t r, size_t l);  //|P| = r mod l
    static uint64_t padd_proc3(uint8_t* P, size_t r, size_t l);  //|P| = r mod l
    static uint64_t padd_proc3(uint64_t P, size_t r, size_t l);  //|P| = r mod l
};