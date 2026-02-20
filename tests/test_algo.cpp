#include <gtest/gtest.h>
#include <algorithm>
#include <random>
#include "buchberger.h"
#include "algo_utils.h"
#include "monomial.h"
#include "orderings.h"
#include "polynomial.h"

using gb::Monomial;
using gb::Polynomial;
using gb::Term;

Term GetRandomTerm(size_t size, int seed) {
    std::mt19937 gen(seed);

    std::uniform_int_distribution distrib(1, 10000);

    int numerator = distrib(gen);
    int denominator = distrib(gen);
    Term::Coeff coeff(numerator, denominator);

    std::uniform_int_distribution distrib_degree(0, 15);
    std::vector<Monomial::Degree> degrees(size);
    for (size_t i = 0; i < size; ++i) {
        degrees[i] = distrib_degree(gen);
    }
    return Term(coeff, {degrees});
}

TEST(AlgoTest, Utils) {
    Term lcm = gb::LCM(Term(2, {1, 2, 3}), Term(5, {0, 4, 5, 6}));
    EXPECT_EQ(lcm, Term({1, 4, 5, 6}));

    Polynomial f = Polynomial() + Monomial{2, 1} - Monomial{1, 0, 2};
    Polynomial g = Polynomial() + Monomial{1, 2} - Monomial{0, 1, 1};
    EXPECT_EQ(gb::LCM(f.GetLeadingTerm(), g.GetLeadingTerm()), Term(Monomial({2, 2})));

    EXPECT_EQ(gb::CalcSPolynomial(f, g), -gb::CalcSPolynomial(g, f));
    Polynomial s = gb::CalcSPolynomial(f, g);
    Polynomial answer = Polynomial() + Monomial{1, 1, 1} - Monomial{1, 1, 2};
    EXPECT_EQ(s, answer);
}

TEST(AlgoTest, ReductionWithGroebnerBasis) {
    Polynomial f1 = Polynomial() + Term(2, {1, 1}) + Term(4, {1, 0, 1}) + Term({0, 1, 2});
    Polynomial f2 = Polynomial() + Term(4, {1, 0, 2}) + Term({0, 1, 3}) + Term{4, {}};
    Polynomial f3 = Polynomial() + Term({0, 2, 3}) + Term(4, {0, 1}) + Term(8, {0, 0, 1});

    // {f1, f2, f3} -- Groebner System with Lex ordering
    std::vector<Polynomial<gb::LexOrder>> system{f1, f2, f3};

    Polynomial cur = GetRandomTerm(30, 13284) * f1 + GetRandomTerm(30, 58692565) * f2 +
                     GetRandomTerm(30, 82582456) * f3;
    gb::ReducePolynomialBySystem(cur, system);
    EXPECT_EQ(cur, Polynomial());
}

TEST(AlgoTest, BasicBuchberger) {
    Polynomial f1 = Polynomial() + Term(2, {1, 1}) + Term(4, {1, 0, 1}) + Term({0, 1, 2});
    Polynomial f2 = Polynomial() + Term(4, {1, 0, 2}) + Term({0, 1, 3}) + Term{4, {}};
    Polynomial f3 = Polynomial() + Term({0, 2, 3}) + Term(4, {0, 1}) + Term(8, {0, 0, 1});

    // {f1, f2, f3} -- Groebner System with Lex ordering
    std::vector<Polynomial<gb::LexOrder>> system{f1, f2, f3};
    std::vector<Polynomial<gb::LexOrder>> system_copy = system;
    gb::FindGroebnerBasis(system);
    EXPECT_EQ(system, system_copy);
    EXPECT_TRUE(gb::IsGroebnerBasis(system));

    f1 = Polynomial() + Term(1, {2, 1}) + Term(2, {0, 0, 2});
    f2 = Polynomial() + Term(1, {0, 2}) - Term(1, {0, 1, 1});
    system = {f1, f2};
    EXPECT_FALSE(gb::IsGroebnerBasis(system));
    gb::FindGroebnerBasis(system);
    EXPECT_EQ(system.size(), 4);
    EXPECT_TRUE(gb::IsGroebnerBasis(system));
    f3 = Polynomial() + Term(2, {0, 1, 2}) - Term(2, {0, 0, 3});
    Polynomial f4 = Polynomial() + Term(1, {2, 0, 3}) + Term(2, {0, 0, 4});
    EXPECT_EQ(system[2], f3);
    EXPECT_EQ(system[3], f4);
}
