#include "random.h"

gb::Rational Random::Rational(gb::Rational::Numeric limit) {
    std::uniform_int_distribution<gb::Numeric> num_dist(-limit, limit);
    gb::Rational::Numeric numerator = num_dist(gen_);
    gb::Rational::Numeric denominator = 1;
    return {numerator, denominator};
}

gb::Monomial Random::Monomial(size_t size, gb::Monomial::Degree degree_limit) {
    std::vector<gb::Monomial::Degree> degrees(size);

    std::uniform_int_distribution<gb::Monomial::Degree> dist(0, degree_limit);

    for (size_t i = 0; i != size; ++i) {
        degrees[i] = dist(gen_);
    }

    return {std::move(degrees)};
}

gb::Term<gb::Rational> Random::Term(size_t monomial_size, gb::Rational::Numeric limit) {
    gb::Rational coeff = Rational(limit);
    gb::Monomial monomial = Monomial(monomial_size);

    return {coeff, std::move(monomial)};
}
