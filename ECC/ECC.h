#pragma once
#include "BigInteger.h"
#include "common.h"
#include "hash/hash.h"
#include "params.h"

class ECC {
    ECP::Params param_set;
    Streebog hash;
    BigInteger k, e, r, s;
    prnd_func rnd;
    size_t size;
    ECP::Point P;

    void gen_new_k() {
        k.fit_to_size(size);
        while (true) {
            rnd(k.get_data(), k.size());
            if (BigInteger::ZERO < k && k < param_set.q) {
                ECP::Point C = P * k;
                r = C.first % param_set.q;
                if (r != BigInteger::ZERO) {
                    break;
                }
            }
        }
    }

  public:
    ECC(prnd_func rnd, const ECP::Params p = ECP::setC)
        : param_set(p), hash(NULL, p.is_512), rnd(rnd),
          size((param_set.is_512) ? 64 : 32),
          P(param_set.u, param_set.v, param_set) {}

    void update(uint8_t *data, size_t len) { hash.update(data, len); }

    void sign(uint8_t *res, const BigInteger &d) {
        if (!res) {
            return;
        }

        BigInteger h(size);
        hash.finish(h.get_data());
        e = h % param_set.q;
        if (e == BigInteger::ZERO) {
            e = BigInteger::ONE;
        }

        while (true) {
            gen_new_k();
            s = (r * d + k * e) % param_set.q;
            if (s != BigInteger::ZERO) {
                break;
            }
        }

        r.fit_to_size(size);
        s.fit_to_size(size);

        reverse(r.get_data(), r.get_data() + size);
        reverse(s.get_data(), s.get_data() + size);

        memcpy(res, r.get_data(), size);
        memcpy(res + size, s.get_data(), size);
    }

    bool verify(uint8_t *signature, const ECP::Point &Q) {
        if (!signature) {
            return false;
        }

        r = BigInteger(size);
        s = BigInteger(size);

        memcpy(r.get_data(), signature, size);
        reverse(r.get_data(), r.get_data() + size);
        memcpy(s.get_data(), signature + size, size);
        reverse(s.get_data(), s.get_data() + size);

        if (!((BigInteger::ZERO < r && r < param_set.q) &&
              (BigInteger::ZERO < s && s < param_set.q))) {
            return false;
        }

        BigInteger h(size);
        hash.finish(h.get_data());

        e = h % param_set.q;
        if (e == BigInteger::ZERO) {
            e = BigInteger::ONE;
        }

        BigInteger v = e.get_inv_mod(param_set.q);

        BigInteger z1 = (s * v) % param_set.q;
        BigInteger z2 = (-(r * v)) % param_set.q;

        ECP::Point C = P * z1 + Q * z2;

        BigInteger R = C.first % param_set.q;

        return R == r;
    }
};