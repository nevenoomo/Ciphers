#pragma once
#include "common.h"
#include "params.h"

typedef int (*prnd_func)(uint8_t *buf, int num);

class KeyGenerator {
    prnd_func rnd;
    ECP::Params params;

  public:
    KeyGenerator(prnd_func rnd, const ECP::Params & params) : rnd(rnd), params(params) {}

    void gen_priv_key(BigInteger& d) const{
        bool correct = false;
        BigInteger tmp(params.q.size());
        while(!correct){
            rnd(tmp.get_data(), tmp.size());
            if (0 < tmp && tmp < params.q){
                correct = true;
            }
        }  
        d = tmp;
    }

    void gen_pub_key(ECP::Point& Q, const BigInteger& d) const{
        Q = params.P_edwards*d;
    }
}