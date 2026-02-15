#include <gtest/gtest.h>
#include "term.h"

using gb::Monomial;
using gb::Term;

TEST(TermTest, Constructors) {
    Term t;
    EXPECT_EQ(t.GetCoeff(), 0);
    EXPECT_EQ(t.GetMonomial(), gb::Monomial());

    Monomial m = Monomial{1, 2, 3};

    t = Term(1, std::move(m));
    EXPECT_EQ(t.GetMonomial(), Monomial({1, 2, 3}));
}

TEST(TermTest, Comparison) {
    Monomial m1 = Monomial{1, 2, 3};
    Monomial m2 = Monomial({3});

    Term t1(3, m1);
    Term t2(4, m2);
    Term t3(0, m1 * m2 * m2);
    Term t4(0, Monomial({3, 5, 6, 7}));

    EXPECT_NE(t1, t2);
    EXPECT_NE(t1, t3);
    EXPECT_NE(t2, t3);
    EXPECT_EQ(t1 * t2, t2 * t1);
    EXPECT_EQ(t3, t4);
}

TEST(TermTest, Arithmetics) {
    Monomial m1 = Monomial{1, 2, 3};
    Monomial m2 = Monomial({3});

    Term t1(3, m1);
    Term t2(4, m2);
    Term t = t1 * t2;
    EXPECT_EQ(t.GetCoeff(), 12);
    EXPECT_EQ(t.GetMonomial(), Monomial({4, 2, 3}));
    t /= t1;
    EXPECT_EQ(t, t2);
}
