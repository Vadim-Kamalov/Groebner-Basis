#include <gtest/gtest.h>
#include "prime_field.h"

using gb::IsPrime;
using gb::Numeric;
using gb::PrimeField;

TEST(PrimeFieldTest, IsPrime) {
    static_assert(!IsPrime(-100));
    static_assert(!IsPrime(1));
    static_assert(!IsPrime(0));
    static_assert(IsPrime(2));
    static_assert(IsPrime(3));
    static_assert(!IsPrime(4));
    static_assert(IsPrime(5));
    static_assert(!IsPrime(6));
    static_assert(IsPrime(7));
    static_assert(!IsPrime(8));
    static_assert(!IsPrime(9));
    static_assert(!IsPrime(10));
    static_assert(IsPrime(11));
    static_assert(!IsPrime(12));
    static_assert(IsPrime(13));

    static_assert(IsPrime(132459389));
    static_assert(IsPrime(57178292741));
    static_assert(!IsPrime(1000000003));
}

TEST(PrimeFieldTest, Constructors) {
    constexpr Numeric p = 1471;
    using Field = PrimeField<p>;

    Field num;
    EXPECT_EQ(num.Value(), 0);

    num = Field(1948);
    EXPECT_EQ(num.Value(), 1948 % 1471);

    num = Field(1948 - 100 * p);
    EXPECT_EQ(num.Value(), 1948 % 1471);
}

TEST(PrimeFieldTest, Arithmetics) {
    constexpr Numeric p = 1471;
    using Field = PrimeField<p>;

    Field num;
    EXPECT_EQ(num + num, num);

    num += 1;
    EXPECT_EQ(num, 1);
    EXPECT_EQ(num, p + 1);

    for (Numeric i = 1; i != p; ++i) {
        num += 1;
    }
    EXPECT_EQ(num, 0);

    EXPECT_EQ(Field(19) * Field(103), 19 * 103);

    for (Field k = 1; k != p; ++k) {
        Field inv = k.Inverse();
        EXPECT_EQ(k * inv, 1);
    }
}
