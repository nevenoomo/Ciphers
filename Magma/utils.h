#pragma once
#include "common.h"

class Utils {
   public:
    static uint64_t filesize(istream & s);
    static uint8_t* convert_to_arr(uint64_t P);
    static uint64_t convert_to_num(uint8_t* P);
    static uint64_t padd_proc2(uint8_t* P, size_t r, size_t l);  //|P| = r mod l
    static uint64_t padd_proc2(uint64_t P, size_t r, size_t l);  //|P| = r mod l
    static uint64_t padd_proc3(uint8_t* P, size_t r, size_t l);  //|P| = r mod l
    static uint64_t padd_proc3(uint64_t P, size_t r, size_t l);  //|P| = r mod l
};