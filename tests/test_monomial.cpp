#include <gtest/gtest.h>
#include "monomial.h"

using gb::Monomial;
using Degree = gb::Monomial::Degree;

TEST(MonomialTest, Constructors) {
    Monomial m;
    EXPECT_EQ(m.Degrees().size(), 0);
    EXPECT_EQ(m.DegreeOf(0), 0);
    EXPECT_EQ(m.DegreeSum(), 0);

    m = Monomial{1, 2, 3};
    EXPECT_EQ(m.DegreeSum(), 6);
    EXPECT_EQ(m.Degrees().size(), 3);
    EXPECT_EQ(m.DegreeOf(0), 1);
    EXPECT_EQ(m.DegreeOf(1), 2);
    EXPECT_EQ(m.DegreeOf(2), 3);
    EXPECT_EQ(m.DegreeOf(3), 0);

    Monomial m2{1, 2, 3, 0, 0, 0, 0};
    EXPECT_EQ(m2.Degrees().size(), 3);
    EXPECT_EQ(m.DegreeSum(), 6);
    EXPECT_EQ(m2, m);
}

TEST(MonomialTest, Comparison) {
    EXPECT_EQ(Monomial({0, 0, 0, 0}), Monomial());
    EXPECT_EQ(Monomial({1, 2}), Monomial({1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}));
    EXPECT_NE(Monomial({1, 2}), Monomial({1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1}));
}

TEST(MonomialTest, Arithmetics) {
    Monomial m{1, 2, 3};
    Monomial m2{1, 4, 5};
    Monomial m3 = m2;
    EXPECT_EQ(m.DegreeSum(), 6);
    EXPECT_EQ(m2.DegreeSum(), 10);
    EXPECT_TRUE(m2.IsDivisibleBy(m));
    EXPECT_FALSE(m.IsDivisibleBy(m2));

    m2 /= m;
    EXPECT_TRUE(m2.Degrees() == std::vector<Degree>({0, 2, 2}));
    EXPECT_EQ(m2.DegreeSum(), 4);
    EXPECT_EQ(m2 * m, m3);

    m2 /= m2;
    EXPECT_TRUE(m2.Degrees().empty());
}

TEST(MonomialTest, ArithmeticsWithDifferentSizes) {
    Monomial m{0, 0, 0, 0, 0, 0};
    EXPECT_TRUE(m.Degrees().empty());

    m = {1, 2, 3, 0, 0, 0};
    EXPECT_EQ(m * Monomial(), m);
    EXPECT_EQ(Monomial() * m, m);
    ASSERT_TRUE(m.IsDivisibleBy(Monomial()));
    EXPECT_EQ(m / Monomial(), m);

    Monomial m2{4};
    EXPECT_TRUE((m * m2).Degrees() == std::vector<Degree>({5, 2, 3}));
    EXPECT_TRUE((m2 * m).Degrees() == std::vector<Degree>({5, 2, 3}));

    Monomial m3{0, 0, 0, 1, 2, 3};
    ASSERT_FALSE(m.IsDivisibleBy(m3));
    ASSERT_FALSE(m3.IsDivisibleBy(m));
    EXPECT_EQ(m3 * m, m * m3);
    EXPECT_EQ(m2 * m2 * m3 * m, m2 * m3 * m * m2);

    Monomial m4;
    EXPECT_EQ(m4 * m, m * m4);
    EXPECT_EQ(m4 * m, m);
    EXPECT_EQ(m4 * m4 * m4 * m4 * m4 * m4, m4);
}
