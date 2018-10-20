#pragma once
#include "common.h"

class Padder {
   public:
    static uint64_t procedure2(uint64_t P, size_t r, size_t l);  //|P| = r mod l
    static uint64_t procedure3(uint64_t P, size_t r, size_t l);  //|P| = r mod l    
};