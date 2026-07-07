#include <gtest/gtest.h>
#include "algo_utils.h"
#include "monomial.h"
#include "orderings.h"
#include "polynomial.h"
#include "buchberger.h"

using Poly = gb::Polynomial<gb::Rational, gb::GrevlexOrder>;
using Term = gb::Term<gb::Rational>;
using gb::Monomial;

using algo = gb::Algorithm<gb::Rational, gb::GrevlexOrder>;
using buchberger = gb::Buchberger<gb::Rational, gb::GrevlexOrder>;

std::vector<Poly> Cyclic(size_t n) {
    if (n == 0) {
        return {};
    }
    std::vector<Poly> result;
    result.reserve(n);

    for (size_t i = 1; i != n; ++i) {
        Poly p;
        for (size_t k = 0; k != n; ++k) {
            std::vector<gb::Monomial::Degree> degrees(n, 0);
            for (size_t j = 0; j != i; ++j) {
                degrees[(k + j) % n] = 1;
            }
            p += Term(1, std::move(degrees));
        }
        result.emplace_back(std::move(p));
    }
    std::vector<gb::Monomial::Degree> degrees(n, 1);
    Poly p;
    p += Term(1, std::move(degrees));
    p += Term(-1);
    result.emplace_back(std::move(p));

    return result;
}

// cyclic-4 answer (from Singular):
// G[1]=x1+x2+x3+x4
// G[2]=x2^2+2*x2*x4+x4^2
// G[3]=x2*x3^2+x3^2*x4-x2*x4^2-x4^3
// G[4]=x2*x3*x4^2+x3^2*x4^2-x2*x4^3+x3*x4^3-x4^4-1
// G[5]=x2*x4^4+x4^5-x2-x4
// G[6]=x3^3*x4^2+x3^2*x4^3-x3-x4
// G[7]=x3^2*x4^4+x2*x3-x2*x4+x3*x4-2*x4^2

std::vector<Poly> makeCyclic4GroebnerBasisGrevlex() {
    std::vector<Poly> basis;

    // G[1] = x1 + x2 + x3 + x4
    Poly g1;
    g1 += Term(1, Monomial({1, 0, 0, 0}));
    g1 += Term(1, Monomial({0, 1, 0, 0}));
    g1 += Term(1, Monomial({0, 0, 1, 0}));
    g1 += Term(1, Monomial({0, 0, 0, 1}));
    basis.push_back(g1);

    // G[2] = x2^2 + 2*x2*x4 + x4^2
    Poly g2;
    g2 += Term(1, Monomial({0, 2, 0, 0}));
    g2 += Term(2, Monomial({0, 1, 0, 1}));
    g2 += Term(1, Monomial({0, 0, 0, 2}));
    basis.push_back(g2);

    // G[3] = x2*x3^2 + x3^2*x4 - x2*x4^2 - x4^3
    Poly g3;
    g3 += Term(1, Monomial({0, 1, 2, 0}));
    g3 += Term(1, Monomial({0, 0, 2, 1}));
    g3 += Term(-1, Monomial({0, 1, 0, 2}));
    g3 += Term(-1, Monomial({0, 0, 0, 3}));
    basis.push_back(g3);

    // G[4] = x2*x3*x4^2 + x3^2*x4^2 - x2*x4^3 + x3*x4^3 - x4^4 - 1
    Poly g4;
    g4 += Term(1, Monomial({0, 1, 1, 2}));
    g4 += Term(1, Monomial({0, 0, 2, 2}));
    g4 += Term(-1, Monomial({0, 1, 0, 3}));
    g4 += Term(1, Monomial({0, 0, 1, 3}));
    g4 += Term(-1, Monomial({0, 0, 0, 4}));
    g4 += Term(-1, Monomial());
    basis.push_back(g4);

    // G[5] = x2*x4^4 + x4^5 - x2 - x4
    Poly g5;
    g5 += Term(1, Monomial({0, 1, 0, 4}));
    g5 += Term(1, Monomial({0, 0, 0, 5}));
    g5 += Term(-1, Monomial({0, 1, 0, 0}));
    g5 += Term(-1, Monomial({0, 0, 0, 1}));
    basis.push_back(g5);

    // G[6] = x3^3*x4^2 + x3^2*x4^3 - x3 - x4
    Poly g6;
    g6 += Term(1, Monomial({0, 0, 3, 2}));
    g6 += Term(1, Monomial({0, 0, 2, 3}));
    g6 += Term(-1, Monomial({0, 0, 1, 0}));
    g6 += Term(-1, Monomial({0, 0, 0, 1}));
    basis.push_back(g6);

    // G[7] = x3^2*x4^4 + x2*x3 - x2*x4 + x3*x4 - 2*x4^2
    Poly g7;
    g7 += Term(1, Monomial({0, 0, 2, 4}));
    g7 += Term(1, Monomial({0, 1, 1, 0}));
    g7 += Term(-1, Monomial({0, 1, 0, 1}));
    g7 += Term(1, Monomial({0, 0, 1, 1}));
    g7 += Term(-2, Monomial({0, 0, 0, 2}));
    basis.push_back(g7);

    return basis;
}

TEST(AlgoTest, Cyclic4) {
    std::vector<Poly> cyclic = Cyclic(4);
    EXPECT_EQ(cyclic.size(), 4);

    Poly f1 = Poly() + Term(1, {1}) + Term(1, {0, 1}) + Term(1, {0, 0, 1}) + Term(1, {0, 0, 0, 1});
    Poly f2 = Poly() + Term(1, {1, 1}) + Term(1, {0, 1, 1}) + Term(1, {0, 0, 1, 1}) +
              Term(1, {1, 0, 0, 1});
    Poly f3 = Poly() + Term(1, {1, 1, 1}) + Term(1, {0, 1, 1, 1}) + Term(1, {1, 0, 1, 1}) +
              Term(1, {1, 1, 0, 1});
    Poly f4 = Poly() + Term(1, {1, 1, 1, 1}) + Term(-1);

    std::vector<Poly> expected{std::move(f1), std::move(f2), std::move(f3), std::move(f4)};
    EXPECT_EQ(cyclic, expected);

    auto basis = buchberger::FindGroebnerBasis(cyclic);
    EXPECT_TRUE(algo::IsGroebnerBasis(basis));

    basis = algo::ReduceBasis(std::move(basis));

    EXPECT_EQ(basis.size(), 7);

    expected = makeCyclic4GroebnerBasisGrevlex();
    algo::SortSetByMonomials(expected.begin(), expected.end());
    EXPECT_EQ(basis, expected);
}
