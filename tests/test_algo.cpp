#include <gtest/gtest.h>
#include <algorithm>
#include <random>
#include "buchberger.h"
#include "algo_utils.h"
#include "monomial.h"
#include "orderings.h"
#include "polynomial.h"
#include "rational.h"
#include "random.h"

using gb::Monomial;
using Poly = gb::Polynomial<gb::Rational, gb::LexOrder>;
using Term = gb::Term<gb::Rational>;

using algo = gb::Algorithm<gb::Rational, gb::LexOrder>;
using buchberger = gb::Buchberger<gb::Rational, gb::LexOrder>;

static Random rnd;

TEST(AlgoTest, Utils) {
    Term lcm = gb::LCM(Monomial({1, 2, 3}), Monomial({0, 4, 5, 6}));
    EXPECT_EQ(lcm, Term({1, 4, 5, 6}));

    Poly f = Poly() + Monomial{2, 1} - Monomial{1, 0, 2};
    Poly g = Poly() + Monomial{1, 2} - Monomial{0, 1, 1};
    EXPECT_EQ(gb::LCM(f.LeadingMonomial(), g.LeadingMonomial()), Monomial({2, 2}));

    EXPECT_EQ(algo::SPolynomial(f, g), -algo::SPolynomial(g, f));
    Poly s = algo::SPolynomial(f, g);
    Poly answer = Poly() + Monomial{1, 1, 1} - Monomial{1, 1, 2};
    EXPECT_EQ(s, answer);
}

TEST(AlgoTest, ReduceOnce) {
    // g = x1x2x3
    // f1 = x1x2 - x4
    // f2 = x1x3 - x4
    Poly g = Term{Monomial{1, 1, 1}};
    Poly f1 = Poly() + Term{Monomial{1, 1}} + Term(-1, {0, 0, 0, 1});
    Poly f2 = Poly() + Term{Monomial{1, 0, 1}} + Term(-1, {0, 0, 0, 1});

    Poly g_copy = g;
    EXPECT_EQ(algo::Status::ReducedOnce, algo::ReduceOnce(f1, &g));
    Poly answer = Term{Monomial{0, 0, 1, 1}};
    EXPECT_EQ(g, answer);

    EXPECT_EQ(algo::Status::ReducedOnce, algo::ReduceOnce(f2, &g_copy));
    answer = Term{Monomial{0, 1, 0, 1}};
    EXPECT_EQ(g_copy, answer);

    EXPECT_EQ(algo::Status::NoReduction, algo::ReduceOnce(f1, &g));
    EXPECT_EQ(algo::Status::NoReduction, algo::ReduceOnce(f2, &g));
    EXPECT_EQ(algo::Status::NoReduction, algo::ReduceOnce(f1, &g_copy));
    EXPECT_EQ(algo::Status::NoReduction, algo::ReduceOnce(f2, &g_copy));
}

TEST(AlgoTest, ReductionWithGroebnerBasis) {
    Poly f1 = Poly() + Term(2, {1, 1}) + Term(4, {1, 0, 1}) + Term({0, 1, 2});
    Poly f2 = Poly() + Term(4, {1, 0, 2}) + Term({0, 1, 3}) + Term{4, {}};
    Poly f3 = Poly() + Term({0, 2, 3}) + Term(4, {0, 1}) + Term(8, {0, 0, 1});

    // {f1, f2, f3} -- Groebner System with Lex ordering
    std::vector<Poly> system{f1, f2, f3};

    Poly cur = rnd.Term(30) * f1 + rnd.Term(30) * f2 + rnd.Term(30) * f3;
    algo::ReduceBy(system, &cur);
    EXPECT_EQ(cur, Poly());
}

TEST(AlgoTest, BasicBuchberger) {
    Poly f1 = Poly() + Term(2, {1, 1}) + Term(4, {1, 0, 1}) + Term({0, 1, 2});
    Poly f2 = Poly() + Term(4, {1, 0, 2}) + Term({0, 1, 3}) + Term{4, {}};
    Poly f3 = Poly() + Term({0, 2, 3}) + Term(4, {0, 1}) + Term(8, {0, 0, 1});

    // {f1, f2, f3} -- Groebner System with Lex ordering
    std::vector<Poly> system{f1, f2, f3};
    std::vector<Poly> system_copy = system;
    system = buchberger::FindGroebnerBasis(std::move(system));
    EXPECT_EQ(system, system_copy);
    EXPECT_TRUE(algo::IsGroebnerBasis(system));

    f1 = Poly() + Term(1, {2, 1}) + Term(2, {0, 0, 2});
    f2 = Poly() + Term(1, {0, 2}) - Term(1, {0, 1, 1});
    system = {f1, f2};
    EXPECT_FALSE(algo::IsGroebnerBasis(system));
    system = buchberger::FindGroebnerBasis(std::move(system));
    EXPECT_EQ(system.size(), 4);
    EXPECT_TRUE(algo::IsGroebnerBasis(system));
    f3 = Poly() + Term(2, {0, 1, 2}) - Term(2, {0, 0, 3});
    Poly f4 = Poly() + Term(1, {2, 0, 3}) + Term(2, {0, 0, 4});
    EXPECT_EQ(system[2], f3);
    EXPECT_EQ(system[3], f4);
}

TEST(AlgoTest, ComplicatedReductions) {
    constexpr size_t iter_count = 100;
    constexpr size_t terms_count = 50;
    constexpr size_t monomial_size = 30;

    Poly f1 = Poly() + Term(1, {2, 1}) + Term(2, {0, 0, 2});
    Poly f2 = Poly() + Term(1, {0, 2}) - Term(1, {0, 1, 1});

    std::vector<Poly> system{f1, f2};
    system = buchberger::FindGroebnerBasis(std::move(system));

    for (size_t i = 0; i != iter_count; ++i) {
        Poly p;
        for (const Poly& q : system) {
            p += q * rnd.Polynomial<gb::LexOrder>(terms_count, monomial_size);
        }

        algo::ReduceBy(system, &p);
        EXPECT_EQ(p, Poly());
    }
}

static void CheckIsBasisReduced(const std::vector<Poly>& set) {
    EXPECT_TRUE(algo::IsGroebnerBasis(set));
    for (const Poly& p : set) {
        EXPECT_FALSE(p.IsZero());
        EXPECT_EQ(p.LeadingCoeff(), 1);
    }

    for (size_t i = 0; i != set.size(); ++i) {
        for (size_t j = 0; j != set.size(); ++j) {
            if (i == j) {
                continue;
            }
            const Poly& a = set[i];
            const Poly& b = set[j];
            const Monomial& leading_monomial = b.LeadingMonomial();
            for (auto it = a.Begin(); it != a.End(); ++it) {
                EXPECT_FALSE(it.Monomial().IsDivisibleBy(leading_monomial));
            }
        }
    }
}

TEST(AlgoTest, ReduceBasis) {
    constexpr size_t iter_count = 100;
    constexpr size_t terms_count = 50;
    constexpr size_t monomial_size = 3;
    constexpr size_t random_systems_count = 1;
    constexpr size_t random_systems_size = 5;

    Poly f1 = Poly() + Term(1, {2, 1}) + Term(1, {0, 0, 2});
    Poly f2 = Poly() + Term(1, {0, 2}) - Term(1, {0, 1, 1});

    std::vector<Poly> system{f1, f2};
    system = buchberger::FindGroebnerBasis(std::move(system));

    std::vector<Poly> reduced = algo::ReduceBasis(system);
    EXPECT_NE(reduced, system);
    EXPECT_LE(reduced.size(), system.size());
    CheckIsBasisReduced(reduced);

    for (Poly p : system) {
        algo::ReduceBy(reduced, &p);
        EXPECT_EQ(p, Poly());
    }

    Poly test = system[0] * Term({1, 2, 3}) + system[1] * Term({0, 0, 3}) +
                system[2] * Term({0, 0, 0, 5}) + system[3] * Term({1, 6, 3, 2, 4, 9});
    algo::ReduceBy(reduced, &test);

    for (size_t i = 0; i != iter_count; ++i) {
        Poly p;
        Term t = rnd.Term(monomial_size);
        for (const Poly& q : system) {
            p += q * rnd.Polynomial<gb::LexOrder>(1, 1);
        }

        algo::ReduceBy(reduced, &p);
        EXPECT_EQ(p, Poly());
    }

    for (size_t i = 0; i != random_systems_count; ++i) {
        system = {};
        system.reserve(random_systems_size);
        for (size_t j = 0; j != random_systems_size; ++j) {
            system.push_back(rnd.Polynomial<gb::LexOrder>(2, monomial_size));
        }

        system = buchberger::FindGroebnerBasis(std::move(system));
        system = algo::ReduceBasis(std::move(system));

        CheckIsBasisReduced(system);
    }
}
