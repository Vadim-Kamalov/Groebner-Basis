#pragma once

#include "rational.h"
#include "monomial.h"

namespace gb {
class Term {
public:
    using Coeff = gb::Rational;

    Term() = default;
    Term(Coeff coeff);
    Term(const Monomial& monomial);
    Term(Monomial&& monomial);
    Term(Coeff coeff, const Monomial& monomial);
    Term(Coeff coeff, Monomial&& monomial);

    Term(const Term&) = default;
    Term(Term&&) = default;
    Term& operator=(const Term&) = default;
    Term& operator=(Term&&) noexcept = default;
    ~Term() = default;

    Coeff GetCoeff() const;
    const Monomial& GetMonomial() const;

    Term operator-() const;

    Term& AddCoeff(Coeff other);

    Term& operator*=(const Term& other);
    Term& operator/=(const Term& other);

    friend Term operator*(const Term& lhs, const Term& rhs);
    friend Term operator/(const Term& lhs, const Term& rhs);

    friend bool operator==(const Term& lhs, const Term& rhs);
    friend bool operator!=(const Term& lhs, const Term& rhs);

private:
    Coeff coeff_{};
    Monomial monomial_{};
};
}  // namespace gb
