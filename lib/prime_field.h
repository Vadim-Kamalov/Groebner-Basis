#pragma once

#include <cassert>
#include <cstdint>
namespace gb {
using Numeric = int64_t;

constexpr bool IsPrime(Numeric n) {
    if (n <= 1) {
        return false;
    }

    for (Numeric k = 2; k * k <= n; ++k) {
        if (n % k == 0) {
            return false;
        }
    }

    return true;
}

constexpr Numeric BinPow(Numeric a, Numeric power, Numeric modulus) {
    a = a % modulus;
    Numeric result = 1;

    while (power > 0) {
        if (power % 2 == 1) {
            result = (result * a) % modulus;
        }
        a = (a * a) % modulus;
        power /= 2;
    }

    return result;
}

template <Numeric p>
    requires(IsPrime(p))
class PrimeField {
public:
    PrimeField() : value_() {
    }

    PrimeField(Numeric num) : value_(num % p) {
        if (value_ < 0) {
            value_ += p;
        }
    }

    Numeric Value() const {
        return value_;
    }

    PrimeField Inverse() const {
        assert(value_ != 0);

        return BinPow(value_, p - 2, p);
    }

    PrimeField& operator+=(PrimeField other) {
        value_ += other.value_;
        if (value_ >= p) {
            value_ -= p;
        }
        return *this;
    }

    PrimeField& operator-=(PrimeField other) {
        value_ -= other.value_;
        if (value_ < 0) {
            value_ += p;
        }
        return *this;
    }

    PrimeField& operator*=(PrimeField other) {
        value_ *= other.value_;
        value_ = value_ % p;
        return *this;
    }

    PrimeField& operator/=(PrimeField other) {
        assert(other.value_ != 0);

        return *this *= other.Inverse();
    }

    PrimeField operator-() const {
        if (value_ == 0) {
            return PrimeField(0);
        }
        return PrimeField(p - value_);
    }

    PrimeField& operator++() {
        *this += 1;
        return *this;
    }

    PrimeField operator++(int) {
        PrimeField tmp = *this;
        ++(*this);
        return tmp;
    }

    PrimeField& operator--() {
        *this -= 1;
        return *this;
    }

    PrimeField operator--(int) {
        PrimeField tmp = *this;
        --(*this);
        return tmp;
    }

    friend bool operator==(PrimeField lhs, PrimeField rhs) {
        return lhs.value_ == rhs.value_;
    }

    friend bool operator!=(PrimeField lhs, PrimeField rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(PrimeField lhs, PrimeField rhs) {
        return lhs.value_ < rhs.value_;
    }

    friend bool operator>(PrimeField lhs, PrimeField rhs) {
        return rhs.value_ < lhs.value_;
    }

    friend bool operator<=(PrimeField lhs, PrimeField rhs) {
        return !(lhs.value_ > rhs.value_);
    }

    friend bool operator>=(PrimeField lhs, PrimeField rhs) {
        return !(lhs.value_ < rhs.value_);
    }

    friend PrimeField operator+(PrimeField lhs, PrimeField rhs) {
        PrimeField tmp(lhs);
        tmp += rhs;
        return tmp;
    }

    friend PrimeField operator-(const PrimeField& lhs, const PrimeField& rhs) {
        PrimeField tmp(lhs);
        tmp -= rhs;
        return tmp;
    }

    friend PrimeField operator*(const PrimeField& lhs, const PrimeField& rhs) {
        PrimeField tmp(lhs);
        tmp *= rhs;
        return tmp;
    }

    friend PrimeField operator/(const PrimeField& lhs, const PrimeField& rhs) {
        PrimeField tmp(lhs);
        tmp /= rhs;
        return tmp;
    }

private:
    Numeric value_;
};
}  // namespace gb
