#pragma once
#include "common.h"
#include "hash_data.h"

class Streebog {
    V512 h, N, Sigma;
    FILE *in;
    bool is_512;

    void add_in_ring(V512 a, V512 b, V512 dest) {
        uint16_t overrun = 0;

        for (size_t i = 0; i < BLOCK_SIZE; i++) {
            overrun = a[i] + b[i] + (overrun >> 8);
            dest[i] = (uint8_t)overrun;
        }
    }

    void add_in_ring(V512 a, uint32_t b, V512 dest) {
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
            b += (uint32_t)a[i];
            dest[i] = b;
            b >>= 8;
        }
    }

    void xor512(V512 a, V512 b, V512 dest) {
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
            dest[i] = a[i] ^ b[i];
        }
    }

    void X(V512 k, V512 a, V512 dest) { xor512(k, a, dest); }

    void S(V512 a) {
        for (int i = BLOCK_SIZE - 1; i >= 0; i--)
            a[i] = PI[a[i]];
    }

    void P(V512 a) {
        V512 tmp;
        for (int i = BLOCK_SIZE - 1; i >= 0; i--)
            tmp[i] = a[TAU[i]];
        memcpy(a, tmp, BLOCK_SIZE);
    }

    void L(V512 a) {
        uint64_t *tmp1 = (uint64_t *)a;
        uint64_t tmp2[8];
        memset(tmp2, 0x00, BLOCK_SIZE);
        for (int i = 7; i >= 0; i--) {
            for (int j = BLOCK_SIZE - 1; j >= 0; j--) {
                if (((tmp1[i] >> j) & 1) ==
                    1) { // check the val of bit, then xor
                    tmp2[i] ^= A[BLOCK_SIZE - 1 - j];
                }
            }
        }
        memcpy(a, tmp2, BLOCK_SIZE);
    }

    void get_key(V512 K, int i) { // i == 1..13
        xor512(K, C[i - 1], K);
        S(K);
        P(K);
        L(K);
    }

    void E(V512 K, V512 m, V512 dest) {
        X(K, m, dest);
        for (size_t i = 1; i < 13; i++) {
            S(dest);
            P(dest);
            L(dest);
            get_key(K, i);
            X(K, dest, dest);
        }
    }

    void g(V512 N, V512 h, V512 m) { // return V512 in h
        V512 K, tmp;
        // LPS(h ^ N)
        xor512(h, N, K);
        S(K);
        P(K);
        L(K);

        // E(LPS(h ^ N), m)
        E(K, m, tmp);

        // E(LPS(h ^ N), m) ^ h ^ m
        xor512(tmp, h, tmp);
        xor512(tmp, m, h);
    }

    void Part2(V512 m) { // here we exect |m| == 64b
        g(N, h, m);
        add_in_ring(N, 512, N);
        add_in_ring(Sigma, m, Sigma);
    }

    void
    Part3(V512 m,
          size_t len) { // here we exect |M| == len < 64b in m(starting at 0)
        memset(m + len, 0x0, BLOCK_SIZE - len);
        m[len] = 0x01;
        g(N, h, m);
        add_in_ring(N, len * 8, N);
        add_in_ring(Sigma, m, Sigma);
        V512 zero = {0};
        g(zero, h, N);
        g(zero, h, Sigma);
    }

  public:
    Streebog(FILE *in = NULL, bool is_512 = true) : in(in), is_512(is_512) {
        memset(N, 0x0, BLOCK_SIZE);
        memset(Sigma, 0x0, BLOCK_SIZE);
        if (is_512) {
            memcpy(h, IV512, BLOCK_SIZE);
        } else {
            memcpy(h, IV256, BLOCK_SIZE);
        }
    }

    uint8_t *H() {
        if (in == NULL)
            return NULL;
        V512 m;
        size_t bytes_read = 0;
        while ((bytes_read = fread(m, sizeof(uint8_t), BLOCK_SIZE, in)) ==
               BLOCK_SIZE) {
            Part2(m);
        }
        Part3(m, bytes_read);
        uint8_t *ret;

        if (is_512) {
            ret = (uint8_t *)malloc(BLOCK_SIZE);
            memcpy(ret, h, BLOCK_SIZE);
        } else {
            ret = (uint8_t *)malloc(BLOCK_SIZE / 2);
            memcpy(ret, h + BLOCK_SIZE / 2, BLOCK_SIZE / 2);
        }

        return ret;
    }

    void update(V512 m, size_t size) {
        if (size == BLOCK_SIZE) {
            Part2(m);
        } else {
            Part3(m, size);
        }
    }

    uint8_t *finish() {
        uint8_t *ret;

        if (is_512) {
            ret = (uint8_t *)malloc(BLOCK_SIZE);
            memcpy(ret, h, BLOCK_SIZE);
        } else {
            ret = (uint8_t *)malloc(BLOCK_SIZE / 2);
            memcpy(ret, h + BLOCK_SIZE / 2, BLOCK_SIZE / 2);
        }

        return ret;
    }
};