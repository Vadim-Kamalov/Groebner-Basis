#pragma once

#include <cstdint>

namespace gb {
class Rational {
public:
    using Numeric = int64_t;

    Rational() = default;
    Rational(Numeric num);
    Rational(Numeric numerator, Numeric denominator);

    Numeric Numerator() const;
    Numeric Denominator() const;

    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational& operator/=(const Rational& other);

    Rational operator-() const;

    friend bool operator==(const Rational& lhs, const Rational& rhs);
    friend bool operator!=(const Rational& lhs, const Rational& rhs);
    friend bool operator<(const Rational& lhs, const Rational& rhs);
    friend bool operator>(const Rational& lhs, const Rational& rhs);
    friend bool operator<=(const Rational& lhs, const Rational& rhs);
    friend bool operator>=(const Rational& lhs, const Rational& rhs);

    friend Rational operator+(const Rational& lhs, const Rational& rhs);
    friend Rational operator-(const Rational& lhs, const Rational& rhs);
    friend Rational operator*(const Rational& lhs, const Rational& rhs);
    friend Rational operator/(const Rational& lhs, const Rational& rhs);

private:
    void Normalize();

    Numeric numerator_{0};
    Numeric denominator_{1};
};
}  // namespace gb
