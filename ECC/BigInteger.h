#pragma once
#include "common.h"

class BigInteger {
  private:
    vector<uint8_t> data;
    bool plus;

  public:
    static const BigInteger ZERO;
    static const BigInteger ONE;

    BigInteger(vector<uint8_t> data, bool plus = true)
        : data(data), plus(plus) {}
    BigInteger(size_t size = 0, bool plus = true, uint8_t val = 0)
        : data(size, val), plus(plus) {}

    size_t size() const { return data.size(); }

    uint8_t *get_data() { return data.data(); }

    static BigInteger gcd(const BigInteger &a, const BigInteger &b,
                          BigInteger &x, BigInteger &y) {
        if (a == ZERO) {
            x = ZERO;
            y = ONE;
            return b;
        }
        BigInteger x1, y1;
        BigInteger d = gcd(b % a, a, x1, y1);
        x = y1 - (b / a) * x1;
        y = x1;
        return d;
    }

    BigInteger get_inv_mod(const BigInteger &m) const {
        BigInteger x, y;
        BigInteger g = gcd(*this, m, x, y);
        if (g != ONE) {
            throw logic_error("Bad elliptic parametres");
        }
        x = (x % m + m) % m;
        return x;
    }

    BigInteger operator+(const BigInteger &other) const {
        BigInteger a(*this), b(other);
        a.trim();
        b.trim();
        if (!a.plus && b.plus) {
            return b - (-a);
        } else if (a.plus && !b.plus) {
            return a - (-b);
        }

        size_t ret_size = (a.size() > b.size()) ? a.size() : b.size();
        a.fit_to_size(ret_size);
        b.fit_to_size(ret_size);

        uint8_t carry = 0x0;
        for (size_t i = 0; i < ret_size; ++i) {
            uint16_t sum = a.data[i] + b.data[i] + carry;
            carry = sum >> 8;
            a.data[i] = (uint8_t)sum;
        }

        if (carry) {
            a.data.push_back(carry);
        }

        return a;
    }

    BigInteger operator-() const {
        BigInteger ret(*this);
        ret.plus = !ret.plus;
        return ret;
    }

    BigInteger operator-(const BigInteger &other) const {
        BigInteger a(*this), b(other);
        a.trim();
        b.trim();

        if (a.plus != b.plus) {
            return a + (-b);
        } else if (a < b) {
            return -(b - a);
        } else if (a == b) {
            return ZERO;
        }

        size_t ret_size = (a.size() > b.size()) ? a.size() : b.size();
        a.fit_to_size(ret_size);
        b.fit_to_size(ret_size);

        uint8_t borrow = 0;
        for (size_t i = 0; i < ret_size; ++i) {
            uint16_t sub = a.data[i] - b.data[i] - borrow;
            if (sub < 0) {
                borrow = 1;
                sub += 0x100;
            }
            a.data[i] = (uint8_t)sub;
        }

        a.trim();
        return a;
    }

    bool operator==(const BigInteger &other) const {
        BigInteger a(*this), b(other);
        a.trim();
        b.trim();
        if (a.size() != b.size()) {
            return false;
        }

        bool eq = true;
        for (size_t i = 0; i < a.size() && eq; ++i) {
            eq = a.data[i] == b.data[i];
        }

        return eq;
    }

    bool operator!=(const BigInteger &other) const { return !(*this == other); }

    bool operator<(const BigInteger &other) const {
        BigInteger a(*this), b(other);
        a.trim();
        b.trim();
        if (a.plus != b.plus)
            return a.plus < b.plus;

        if (a.plus) {
            if (a.size() < b.size())
                return true;
            else if (a.size() > b.size())
                return false;
        } else {
            if (a.size() < b.size())
                return false;
            else if (a.size() > b.size())
                return true;
        }
        // here we are garantied the same size
        for (int i = a.size() - 1; i >= 0; --i) {
            if (a.data[i] != b.data[i])
                return ((a.data[i] < b.data[i]) == plus);
        }

        return false;
    }

    bool operator>(const BigInteger &other) const { return other < *this; }

    bool operator<=(const BigInteger &other) const {
        return *this < other || *this == other;
    }

    bool operator>=(const BigInteger &other) const {
        return *this > other || *this == other;
    }

    BigInteger operator*(const BigInteger &other) const {
        if (other == ZERO || *this == ZERO) {
            return ZERO;
        }

        BigInteger ret(size() + other.size());
        ret.plus = plus == other.plus;

        for (size_t i = 0; i < size(); ++i) {
            if (data[i] == 0)
                continue;

            uint8_t mcarry = 0;
            for (size_t j = 0, k = i; j < other.size(); ++j, ++k) {
                // k = i + j
                uint16_t val = data[i] * other.data[j] + ret.data[k] + mcarry;

                ret.data[k] = val & 0xFF;
                mcarry = (val >> 8);
            }

            if (mcarry != 0)
                ret.data[i + other.size()] = mcarry;
        }

        ret.trim();
        return ret;
    }

    BigInteger &operator*=(const BigInteger &other) {
        *this = *this * other;
        return *this;
    }

    BigInteger &operator++() {
        *this = *this + ONE;
        return *this;
    }

    BigInteger &operator-=(const BigInteger &other) {
        *this = *this - other;
        return *this;
    }

    BigInteger operator/(const BigInteger &other) const {
        BigInteger a(*this), b(other);
        a.trim();
        b.trim();
        a.plus = b.plus = true;

        if (b == ZERO) {
            throw runtime_error("Cannot divide by zero");
        }else if (a == b) {
            return ONE;
        }else if (a < b) {
            return ZERO;
        }

        BigInteger Q, R;
        Q.fit_to_size(a.size());
        R.fit_to_size(a.size());

        int i = a.size() * 8 - 1;

        while (!a.get_bit_at(i)) {
            i--;
        }

        for (; i >= 0; --i) {
            R.shl1();
            R.setbit(0, a.get_bit_at(i));
            if (R >= other) {
                R -= other;
                Q.setbit(i, 1);
            }
        }

        Q.trim();
        Q.plus = this->plus == other.plus;
        return Q;
    }

    void shl1() {
        uint8_t carry = 0;
        size_t len = this->size();
        for (size_t i = 0; i < len; ++i) {
            uint8_t tmp = data[i] & 0x80; // save upper bit
            data[i] = (data[i] << 1) | carry;
            carry = tmp;
        }
    }

    void setbit(size_t pos, uint8_t val) {
        data[pos / 8] =
            (data[pos / 8] & ~(1UL << (pos % 8))) | (val << (pos % 8));
    }

    BigInteger operator%(const BigInteger &other) const {
        BigInteger a(*this), b(other);
        a.trim();
        b.trim();
        a.plus = b.plus = true;

        if (b == ZERO) {
            throw runtime_error("Cannot divide by zero");
        }else if (a == b) {
            return ONE;
        }else if (a < b) {
            return ZERO;
        }

        BigInteger Q, R;
        Q.fit_to_size(a.size());
        R.fit_to_size(a.size());

        int i = a.size() * 8 - 1;

        while (!a.get_bit_at(i)) {
            i--;
        }

        for (; i >= 0; --i) {
            R.shl1();
            R.setbit(0, a.get_bit_at(i));
            if (R >= other) {
                R -= other;
                Q.setbit(i, 1);
            }
        }

        R.trim();
        if (plus != other.plus){
            R = b - R;
        }

        return R;
    }

    bool get_bit_at(size_t i) const { return data[i / 8] & (0x1 << (i % 8)); }

    void fit_to_size(size_t s) {
        for (size_t i = size(); i < s; ++i) {
            data.push_back(0x0);
        }
    }

    void trim() {
        while (data.size() > 1 && data[data.size() - 1] == 0)
            data.pop_back();
    }
};

const BigInteger BigInteger::ZERO = BigInteger(1);
const BigInteger BigInteger::ONE = BigInteger(1, true, 1);