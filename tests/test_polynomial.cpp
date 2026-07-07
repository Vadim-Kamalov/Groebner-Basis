#include <gtest/gtest.h>
#include "monomial.h"
#include "orderings.h"
#include "polynomial.h"
#include "rational.h"
#include <algorithm>

using gb::LexOrder;
using gb::Monomial;
using gb::Polynomial;
using Term = gb::Term<gb::Rational>;

using TermsContainer = Polynomial<>::TermsContainer;

TEST(PolynomialTest, ArithmeticsWithTerms) {
    Term t1(Monomial{1, 2, 3});
    Term t2(Monomial{10, 0, 2});
    Term t3(Monomial{0, 0, 0, 6});
    Polynomial p = Polynomial(t1) + t2 + t3;

    p += t1;
    std::vector<Term> answer({t2, gb::Rational{2} * t1, t3});
    EXPECT_TRUE(std::equal(p.Begin(), p.End(), answer.begin(), answer.end()));

    p -= t1;
    p += gb::Rational{4, 3} * t1 * t2 * t3;
    p += gb::Rational{0} * t1 * t3;
    p -= t3;
    answer = {gb::Rational{4, 3} * t1 * t2 * t3, t2, t1};
    EXPECT_TRUE(std::equal(p.Begin(), p.End(), answer.begin(), answer.end()));
}

TEST(PolynomialTest, Arithmetics) {
    Term t1(Monomial{1, 2, 3});
    Term t2(Monomial{10, 0, 2});
    Term t3(Monomial{0, 0, 0, 6});

    Polynomial p = Polynomial(t1) + Polynomial(t2) + Polynomial(t3);

    EXPECT_EQ(p + p, p * 2);
    EXPECT_EQ((p * 1948) / 1948, p);
    EXPECT_EQ((p * 100) * 500, p * 50000);
    EXPECT_EQ(p + p, p * Term(2));
    EXPECT_EQ(p - p, Polynomial());
    EXPECT_TRUE((p - p).IsZero());

    Polynomial p2 = Polynomial(-t1) - t2 - t3;
    Polynomial p3 = -p;
    EXPECT_EQ(p3.Begin().Coeff().Numerator(), p2.Begin().Coeff().Numerator());
    EXPECT_EQ(p3.Begin().Monomial(), p2.Begin().Monomial());
    EXPECT_EQ(p2 + p, Polynomial());
    EXPECT_EQ(p2, -p);
    EXPECT_EQ(-p2, p);
    EXPECT_EQ(-(-p), p);

    Term t4(Monomial{3, 2, 1});
    Term t5(Monomial{4, 5, 6});

    Polynomial q = Polynomial(t4) + t5;

    Polynomial result = p * q;
    Polynomial expected = Polynomial(t1 * t4) + t1 * t5 + t2 * t4 + t2 * t5 + t3 * t4 + t3 * t5;
    EXPECT_EQ(result, expected);
    EXPECT_EQ(result - q * p, Polynomial());
    EXPECT_EQ(result * result - q * q * p * p, Polynomial());
}

TEST(PolynomialTest, OrderingAndNormalize) {
    Term t1({123, 456}, Monomial{1, 2, 3});
    Term t2({1842834, 8245845}, Monomial{10, 0, 2});
    Term t3({-482342134, 83524358}, Monomial{0, 0, 0, 6});

    Polynomial p = Polynomial() + t1 + t2 + t3;
    Polynomial p_copy = p;
    EXPECT_EQ(p.LeadingTerm(), t2);
    EXPECT_EQ(p.LeadingCoeff(), t2.Coeff());
    EXPECT_EQ(p.LeadingMonomial(), t2.Monomial());
    p.Normalize();
    EXPECT_EQ(p.LeadingCoeff(), 1);
    EXPECT_EQ(p.LeadingTerm(), t2.Monomial());
    EXPECT_EQ(p_copy * (1 / t2.Coeff()), p);
}

TEST(PolynomialTest, Iterators) {
    Polynomial p;

    EXPECT_EQ(p.Begin(), p.End());
    Monomial m = {1, 2, 3};

    p += Term(1, m);
    EXPECT_EQ(++p.Begin(), p.End());
    EXPECT_EQ(p.Begin().Coeff(), 1);
    EXPECT_EQ(p.Begin().Monomial(), m);
}
