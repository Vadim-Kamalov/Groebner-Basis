#include <gtest/gtest.h>
#include "orderings.h"
#include "random.h"

using gb::GrevlexOrder;
using gb::GrlexOrder;
using gb::LexOrder;
using gb::Monomial;

static Random rnd;

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

TEST(OrderingsTest, Grevlex) {
    GrevlexOrder order;

    Monomial m1({1, 0, 0});
    Monomial m2({0, 2, 0});
    EXPECT_TRUE(order(m2, m1));

    Monomial a({1, 0, 0});
    Monomial b({0, 1, 0});
    Monomial c({0, 0, 1});
    EXPECT_FALSE(order(b, a));
    EXPECT_FALSE(order(c, a));
    EXPECT_FALSE(order(c, b));

    Monomial m3({1, 2, 0});
    Monomial m4({2, 1, 0});
    EXPECT_FALSE(order(m3, m4));
    EXPECT_TRUE(order(m4, m3));
}

TEST(OrderingsTest, DifferentSizes) {
    constexpr size_t max_size = 100;
    constexpr size_t degree_limit = 3;

    LexOrder lex;
    GrlexOrder grlex;
    GrevlexOrder grevlex;

    for (size_t size1 = 0; size1 != max_size; ++size1) {
        for (size_t size2 = 0; size2 != max_size; ++size2) {
            Monomial a = rnd.Monomial(size1, degree_limit);
            Monomial b = rnd.Monomial(size2, degree_limit);

            if (a == b) {
                EXPECT_FALSE(lex(a, b));
                EXPECT_FALSE(lex(b, a));
                EXPECT_FALSE(grlex(a, b));
                EXPECT_FALSE(grlex(b, a));
                EXPECT_FALSE(grevlex(a, b));
                EXPECT_FALSE(grevlex(b, a));
            } else {
                EXPECT_TRUE(lex(a, b) ^ lex(b, a));
                EXPECT_TRUE(grlex(a, b) ^ grlex(b, a));
                EXPECT_TRUE(grevlex(a, b) ^ grevlex(b, a));
            }
        }
    }
}
