#include <gtest/gtest.h>
#include "monomial.h"
#include "orderings.h"
#include "polynomial.h"

using gb::LexOrder;
using gb::Monomial;
using gb::Polynomial;
using gb::Term;

using TermsContainer = Polynomial<LexOrder>::TermsContainer;

TEST(PolynomialTest, TermsStoredProperly) {
    Term t1(Monomial{1, 2, 3});
    Term t2(Monomial{10, 0, 2});
    Term t3(Monomial{0, 0, 0, 6});
    TermsContainer terms({t1, t2, t3});

    Polynomial p(terms);
    EXPECT_EQ(p.GetTerms(), terms);
    EXPECT_EQ(p.GetLeadingTerm(), t2);

    terms.erase(t2);
    p = Polynomial(terms);
    EXPECT_EQ(p.GetTerms(), terms);
    EXPECT_EQ(p.GetLeadingTerm(), t1);

    terms.erase(t1);
    p = Polynomial(terms);
    EXPECT_EQ(p.GetTerms(), terms);
    EXPECT_EQ(p.GetLeadingTerm(), t3);
}

TEST(PolynomialTest, ArithmeticsWithTerms) {
    Term t1(Monomial{1, 2, 3});
    Term t2(Monomial{10, 0, 2});
    Term t3(Monomial{0, 0, 0, 6});
    TermsContainer terms({t1, t2, t3});

    Polynomial p(terms);
    p += t1;
    Polynomial answer = TermsContainer({gb::Rational{2} * t1, t2, t3});
    EXPECT_EQ(p, answer);

    p = Polynomial(terms);
    p += gb::Rational{4, 3} * t1 * t2 * t3;
    p += gb::Rational{0} * t1 * t3;
    p -= t3;
    answer = TermsContainer({t1, t2, gb::Rational{4, 3} * t1 * t2 * t3});
    EXPECT_EQ(p, answer);
}

TEST(PolynomialTest, Arithmetics) {
    Term t1(Monomial{1, 2, 3});
    Term t2(Monomial{10, 0, 2});
    Term t3(Monomial{0, 0, 0, 6});
    TermsContainer terms({t1, t2, t3});

    Polynomial p(terms);

    EXPECT_EQ(p + p, p * Term(2));
    EXPECT_EQ(p - p, Polynomial());

    Term t4(Monomial{3, 2, 1});
    Term t5(Monomial{4, 5, 6});
    terms = {t4, t5};

    Polynomial q(terms);

    Polynomial result = p * q;
    Polynomial expected = TermsContainer{t1 * t4, t1 * t5, t2 * t4, t2 * t5, t3 * t4, t3 * t5};
    EXPECT_EQ(result, expected);
    EXPECT_EQ(result - q * p, Polynomial());
    EXPECT_EQ(result * result - q * q * p * p, Polynomial());
}

TEST(PolynomialTest, Reduce) {
    // g = x1x2x3
    // f1 = x1x2 - x4
    // f2 = x1x3 - x4
    Polynomial g = TermsContainer{Monomial{1, 1, 1}};
    Polynomial f1 = TermsContainer{Monomial{1, 1}, Term(-1, {0, 0, 0, 1})};
    Polynomial f2 = TermsContainer{Monomial{1, 0, 1}, Term(-1, {0, 0, 0, 1})};

    Polynomial g_copy = g;
    EXPECT_TRUE(g.Reduce(f1));
    Polynomial answer = TermsContainer{Monomial{0, 0, 1, 1}};
    EXPECT_EQ(g, answer);

    EXPECT_TRUE(g_copy.Reduce(f2));
    answer = TermsContainer{Monomial{0, 1, 0, 1}};
    EXPECT_EQ(g_copy, answer);

    EXPECT_FALSE(g.Reduce(f1));
    EXPECT_FALSE(g.Reduce(f2));
    EXPECT_FALSE(g_copy.Reduce(f1));
    EXPECT_FALSE(g_copy.Reduce(f2));
}
