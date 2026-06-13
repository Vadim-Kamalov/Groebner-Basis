#include <gtest/gtest.h>
#include <limits>
#include "rational.h"

using gb::Rational;

TEST(RationalTest, Constructors) {
    Rational r;
    EXPECT_EQ(r.Numerator(), 0);
    EXPECT_EQ(r.Denominator(), 1);

    r = Rational(123456);
    EXPECT_EQ(r.Numerator(), 123456);
    EXPECT_EQ(r.Denominator(), 1);

    r = Rational(1'000'000'007, 999'999'937);
    EXPECT_EQ(r.Numerator(), 1'000'000'007);
    EXPECT_EQ(r.Denominator(), 999'999'937);

    r = Rational(12, 50);
    EXPECT_EQ(r.Numerator(), 6);
    EXPECT_EQ(r.Denominator(), 25);

    r = Rational(-100, -4);
    EXPECT_EQ(r.Numerator(), 25);
    EXPECT_EQ(r.Denominator(), 1);

    r = Rational(100, -4);
    EXPECT_EQ(r.Numerator(), -25);
    EXPECT_EQ(r.Denominator(), 1);

    r = Rational(-100, 4);
    EXPECT_EQ(r.Numerator(), -25);
    EXPECT_EQ(r.Denominator(), 1);
}

TEST(RationalTest, Comparison) {
    EXPECT_EQ(Rational(1, 2), Rational(1 << 19, 1 << 20));
    EXPECT_EQ(Rational(0, -1234), Rational(0));
    EXPECT_NE(Rational(-1), Rational(-1000000, 999999));

    Rational a(19, -21);
    Rational b(1, 2);
    Rational c(5, 6);

    EXPECT_LT(a, b);
    EXPECT_LT(b, c);
    EXPECT_LT(a, c);

    EXPECT_LE(a, b);
    EXPECT_GT(b, a);
    EXPECT_GE(b, a);
};

TEST(RationalTest, Arithmetics) {
    Rational r;
    EXPECT_EQ(r + r, r);
    r += 1;
    EXPECT_EQ(r, Rational(1));
    r += Rational(9, 17);
    EXPECT_EQ(r, Rational(26, 17));

    EXPECT_EQ(Rational(3, 4) - Rational(1, 2), Rational(1, 4));
    EXPECT_EQ(Rational(123456, 78910) * Rational(78910, 123456), 1);

    r = Rational(199933, 3085553);
    EXPECT_EQ(r / r, 1);
}

TEST(RationalTest, Overflow) {
    auto max = std::numeric_limits<Rational::Numeric>::max();
    Rational a(13337, max);
    Rational b(1415891345, max);

    EXPECT_LT(a, b);

    Rational c = a + b;
    EXPECT_EQ(c.Denominator(), max);
    EXPECT_EQ(c.Numerator(), a.Numerator() + b.Numerator());
}
