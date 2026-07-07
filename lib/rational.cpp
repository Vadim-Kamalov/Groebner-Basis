#include <numeric>
#include <cassert>
#include "rational.h"

namespace gb {

Rational::Rational(Numeric num) : Rational(num, 1) {
}

Rational::Rational(Numeric numerator, Numeric denominator)
    : numerator_(numerator), denominator_(denominator) {
    assert(denominator != 0);
    Normalize();
}

Rational::Numeric Rational::Numerator() const {
    return numerator_;
}

Rational::Numeric Rational::Denominator() const {
    return denominator_;
}

Rational& Rational::operator+=(const Rational& other) {
    Numeric lcm = std::lcm(denominator_, other.denominator_);
    numerator_ = numerator_ * (lcm / denominator_) + other.numerator_ * (lcm / other.denominator_);
    denominator_ = lcm;
    Normalize();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    Numeric lcm = std::lcm(denominator_, other.denominator_);
    numerator_ = numerator_ * (lcm / denominator_) - other.numerator_ * (lcm / other.denominator_);
    denominator_ = lcm;
    Normalize();
    return *this;
}

Rational& Rational::operator*=(const Rational& other) {
    numerator_ *= other.numerator_;
    denominator_ *= other.denominator_;
    Normalize();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    assert(other != 0);
    numerator_ *= other.denominator_;
    denominator_ *= other.numerator_;
    Normalize();
    return *this;
}

Rational Rational::operator-() const {
    return {-numerator_, denominator_};
}

bool operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.numerator_ == rhs.numerator_ && lhs.denominator_ == rhs.denominator_;
}

bool operator!=(const Rational& lhs, const Rational& rhs) {
    return !(lhs == rhs);
}

bool operator<(const Rational& lhs, const Rational& rhs) {
    Rational::Numeric lcm = std::lcm(lhs.denominator_, rhs.denominator_);
    return lhs.numerator_ * (lcm / lhs.denominator_) < rhs.numerator_ * (lcm / rhs.denominator_);
}

bool operator>(const Rational& lhs, const Rational& rhs) {
    return rhs < lhs;
}

bool operator<=(const Rational& lhs, const Rational& rhs) {
    return !(lhs > rhs);
}

bool operator>=(const Rational& lhs, const Rational& rhs) {
    return !(lhs < rhs);
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    Rational tmp(lhs);
    tmp += rhs;
    return tmp;
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    Rational tmp(lhs);
    tmp -= rhs;
    return tmp;
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    Rational tmp(lhs);
    tmp *= rhs;
    return tmp;
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    Rational tmp(lhs);
    tmp /= rhs;
    return tmp;
}

void Rational::Normalize() {
    Numeric gcd = std::gcd(numerator_, denominator_);
    numerator_ /= gcd;
    denominator_ /= gcd;

    if (denominator_ < 0) {
        numerator_ *= -1;
        denominator_ *= -1;
    }
}

std::ostream& operator<<(std::ostream& os, const Rational& r) {
    if (r.Denominator() == 1) {
        os << r.Numerator();
    } else {
        os << r.Numerator() << "/" << r.Denominator();
    }
    return os;
}

}  // namespace gb
