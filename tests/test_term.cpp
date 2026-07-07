#include <gtest/gtest.h>
#include "prime_field.h"
#include "rational.h"
#include "term.h"

using gb::Monomial;
using TermRational = gb::Term<gb::Rational>;
using TermPrime = gb::Term<gb::PrimeField<19>>;

TEST(TermTest, Constructors) {
    TermRational t;
    EXPECT_EQ(t.Coeff(), 0);
    EXPECT_EQ(t.Monomial(), gb::Monomial());

    Monomial m = Monomial{1, 2, 3};

    t = TermRational(1, std::move(m));
    EXPECT_EQ(t.Monomial(), Monomial({1, 2, 3}));
}

TEST(TermTest, Comparison) {
    Monomial m1 = Monomial{1, 2, 3};
    Monomial m2 = Monomial({3});

    TermRational t1(3, m1);
    TermRational t2(4, m2);
    TermRational t3(0, m1 * m2 * m2);
    TermRational t4(0, Monomial({3, 5, 6, 7}));

    EXPECT_NE(t1, t2);
    EXPECT_NE(t1, t3);
    EXPECT_NE(t2, t3);
    EXPECT_EQ(t1 * t2, t2 * t1);
    EXPECT_EQ(t3, t4);
}

TEST(TermTest, Arithmetics) {
    Monomial m1 = Monomial{1, 2, 3};
    Monomial m2 = Monomial({3});

    TermRational t1(3, m1);
    TermRational t2(4, m2);
    TermRational t = t1 * t2;
    EXPECT_EQ(t.Coeff(), 12);
    EXPECT_EQ(t.Monomial(), Monomial({4, 2, 3}));
    t /= t1;
    EXPECT_EQ(t, t2);

    TermRational t3(-3, m1);
    EXPECT_EQ(-t1, t3);
    EXPECT_EQ(t1, -t3);
    EXPECT_EQ(-(-t1), t1);
}
