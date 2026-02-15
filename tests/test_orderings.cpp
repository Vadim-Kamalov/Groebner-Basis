#include <gtest/gtest.h>
#include "orderings.h"

using gb::GrlexOrder;
using gb::LexOrder;
using gb::Monomial;

TEST(OrderingsTest, Lex) {
    LexOrder order;

    Monomial m1({1});
    Monomial m2({0, 1});
    Monomial m3({0, 0, 1});
    EXPECT_TRUE(order(m1, m2));
    EXPECT_TRUE(order(m2, m3));
    EXPECT_TRUE(order(m1, m3));
    EXPECT_TRUE(order(m1 * m2, m3 * m2));

    Monomial m4({1, 2});
    EXPECT_TRUE(order(m4, m1));
}

TEST(OrderingsTest, Grlex) {
    GrlexOrder order;

    {
        Monomial m1({1});
        Monomial m2({0, 1});
        Monomial m3({0, 0, 1});
        EXPECT_TRUE(order(m1, m2));
        EXPECT_TRUE(order(m2, m3));
        EXPECT_TRUE(order(m1, m3));
    }

    Monomial m1({1, 2, 3});
    Monomial m2({3, 2, 0});
    EXPECT_TRUE(order(m1, m2));

    Monomial m3({1, 2, 4});
    Monomial m4({1, 1, 5});
    EXPECT_TRUE(order(m3, m4));
}
