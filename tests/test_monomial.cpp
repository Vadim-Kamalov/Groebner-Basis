#include <gtest/gtest.h>
#include "monomial.h"

using gb::Monomial;
using Degree = gb::Monomial::Degree;

TEST(MonomialTest, Constructors) {
    Monomial m;
    EXPECT_EQ(m.GetSize(), 0);
    EXPECT_EQ(m.GetDegree(0), 0);

    m = Monomial{1, 2, 3};
    EXPECT_EQ(m.GetSize(), 3);
    EXPECT_EQ(m.GetDegree(0), 1);
    EXPECT_EQ(m.GetDegree(1), 2);
    EXPECT_EQ(m.GetDegree(2), 3);
    EXPECT_EQ(m.GetDegree(3), 0);
}

TEST(MonomialTest, Comparison) {
    EXPECT_TRUE(Monomial({0, 0, 0, 0}) == Monomial());
    EXPECT_TRUE(Monomial({1, 2}) == Monomial({1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}));
    EXPECT_TRUE(Monomial({1, 2}) != Monomial({1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1}));
}

TEST(MonomialTest, Arithmetics) {
    Monomial m{1, 2, 3};
    Monomial m2{1, 4, 5};
    Monomial m3 = m2;
    EXPECT_TRUE(m2.IsDivisibleBy(m));
    EXPECT_FALSE(m.IsDivisibleBy(m2));

    m2 /= m;
    EXPECT_TRUE(m2.GetDegrees() == std::vector<Degree>({0, 2, 2}));
    EXPECT_EQ(m2 * m, m3);
}

TEST(MonomialTest, ArithmeticsWithDifferentSizes) {
    Monomial m{1, 2, 3, 0, 0, 0};
    EXPECT_EQ(m * Monomial(), m);
    EXPECT_EQ(Monomial() * m, m);
    ASSERT_TRUE(m.IsDivisibleBy(Monomial()));
    EXPECT_EQ(m / Monomial(), m);

    Monomial m2{4};
    EXPECT_TRUE((m * m2).GetDegrees() == std::vector<Degree>({5, 2, 3, 0, 0, 0}));
    EXPECT_TRUE((m2 * m).GetDegrees() == std::vector<Degree>({5, 2, 3, 0, 0, 0}));
}
