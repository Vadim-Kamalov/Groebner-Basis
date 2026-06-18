#pragma once

#include <random>
#include "monomial.h"
#include "polynomial.h"
#include "prime_field.h"
#include "rational.h"
#include "term.h"

class Random {
public:
    template <gb::Numeric p>
    gb::PrimeField<p> PrimeField() {
        std::uniform_int_distribution<gb::Numeric> dist(0, p);
        return {dist(gen_)};
    }
    gb::Rational Rational(gb::Rational::Numeric limit = 10);

    gb::Monomial Monomial(size_t size, gb::Monomial::Degree degree_limit = 5);

    gb::Term<gb::Rational> Term(size_t monomial_size, gb::Rational::Numeric limit = 10);

    template <gb::Numeric p>
    gb::Term<gb::PrimeField<p>> Term(size_t monomial_size) {
        gb::PrimeField<p> coeff = PrimeField<p>();
        gb::Monomial monomial = Monomial(monomial_size);
        return {coeff, std::move(monomial)};
    }

    template <typename Ordering>
    gb::Polynomial<gb::Rational, Ordering> Polynomial(size_t terms_count, size_t monomial_size) {
        gb::Polynomial<gb::Rational, Ordering> result;
        for (size_t i = 0; i < terms_count; ++i) {
            result += Term(monomial_size);
        }

        return result;
    }

    template <gb::Numeric p, typename Ordering>
    gb::Polynomial<gb::PrimeField<p>, Ordering> Polynomial(size_t terms_count,
                                                           size_t monomial_size) {
        gb::Polynomial<gb::PrimeField<p>, Ordering> result;
        for (size_t i = 0; i != terms_count; ++i) {
            result += Term<p>(monomial_size);
        }

        return result;
    }

private:
    std::mt19937 gen_{42};
};
