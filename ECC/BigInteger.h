#include "common.h"
#define ZERO BigInteger(1)
#define ONE BigInteger(1, true, 1)

class BigInteger {
  private:
    vector<uint8_t> data;
    bool plus;

  public:
    BigInteger(vector<uint8_t> data, bool plus = true)
        : data(data), plus(plus) {}
    BigInteger(size_t size = 0, bool plus = true, uint8_t val = 0)
        : data(size, val), plus(plus) {}

    size_t size() const { return data.size(); }

    uint8_t *get_data() { return data.data(); }

    BigInteger get_inv_mod(const BigInteger &modulo) const {}

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

    BigInteger get_inv_mod(const BigInteger& m) const{
        BigInteger x,y;
        BigInteger g = gcd(*this, m, x, y);
        if (g != ONE){
            throw logic_error("Bad elliptic parametres");            
        }
        x = (x % m + m) % m;
        return x;
    }

    BigInteger operator+(const BigInteger &other) const {
        if (!plus && other.plus) {
            return other - (-*this);
        }
        if (plus && !other.plus) {
            return *this - (-other);
        }
        size_t ret_size = (size() > other.size()) ? size() : other.size();
        BigInteger ret(ret_size, plus);

        uint8_t carry = 0x0;
        for (size_t i = 0; i < ret_size; ++i) {
            uint16_t sum = data[i] + other.data[i] + carry;
            carry = sum >> 8;
            ret.data[i] = (uint8_t)sum; // REVIEW endiannes
        }

        if (carry) {
            ret.data.push_back(carry);
        }

        ret.trim();
        return ret;
    }

    BigInteger operator-() const {
        BigInteger ret(*this);
        ret.plus = !ret.plus;
        return ret;
    }

    BigInteger operator-(const BigInteger &other) const {
        if (plus && !other.plus) {
            return *this + other;
        }
        if (!plus && other.plus) {
            return *this + (-other);
        }
        if (*this < other) {
            return -(other - *this);
        }
        if (*this == other) {
            return BigInteger(vector<uint8_t>(1, 0)); // ZERO
        }

        size_t ret_size = (size() > other.size()) ? size() : other.size();
        BigInteger ret(ret_size, plus);

        uint8_t borrow = 0;
        for (size_t i = 0; i < ret_size; ++i) {
            uint16_t sub = data[i] - other.data[i] - borrow;
            if (sub < 0) {
                borrow = 1;
                sub += 0x100;
            }
            ret.data[i] = (uint8_t)sub;
        }

        ret.trim();
        return ret;
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

    bool operator!=(const BigInteger &other) const{
        return ! (*this == other);
    }

    bool operator<(const BigInteger &other) const {
        if (!plus && other.plus)
            return true;
        if (plus && !other.plus)
            return false;
        if (plus) {
            if (size() < other.size())
                return true;
            else if (size() > other.size())
                return false;
        } else {
            if (size() < other.size())
                return false;
            else if (size() > other.size())
                return true;
        }

        bool lt = true;
        for (int i = size() - 1; i >= 0 && lt; --i) {
            lt = (data[i] < other.data[i] == plus);
        }

        return lt;
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
            for (int j = 0, k = i; j < other.size(); ++j, ++k) {
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
        if (*this == other) {
            return ONE;
        }

        if (*this < other) {
            return ZERO;
        }

        BigInteger ret;

        BigInteger left(*this), was(other);
        left.plus = true;
        was.plus = true;
        while (left > was) {
            left -= was;
            ++ret;
        }
        ret.plus = plus == other.plus;

        return ret;
    }

    BigInteger operator%(const BigInteger &other) const {
        if (*this == other) {
            return ONE;
        }

        if (*this < other) {
            return ZERO;
        }

        BigInteger ret;

        BigInteger left(*this), was(other);
        left.plus = true;
        was.plus = true;

        while (left > was) {
            left -= was;
            ++ret;
        }

        if (plus != other.plus) {
            left = other - left;
        }

        return left;
    }

    bool get_bit_at(size_t i) const { return data[i / 8] & (0x1 << (i % 8)); }

    void fit_to_size(size_t s){
        for(size_t i = size(); i < s; ++i){
            data.push_back(0x0);
        }
    }

    void trim() {
        while (data.size() > 1 && data[data.size() - 1] == 0)
            data.pop_back();
    }
};
