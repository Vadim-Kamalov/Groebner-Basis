#pragma once

#include "rational.h"
#include "monomial.h"

namespace gb {
template <typename Coefficient = Rational>
class Term {
public:
    using MonomialT = gb::Monomial;

    Term() = default;
    Term(Coefficient coeff) : coeff_(coeff) {
    }
    Term(Coefficient coeff, const MonomialT& monomial) : coeff_(coeff), monomial_(monomial) {
        if (coeff_ == 0) {
            monomial_ = {};
        }
    }
    Term(Coefficient coeff, MonomialT&& monomial) : coeff_(coeff), monomial_(std::move(monomial)) {
        if (coeff_ == 0) {
            monomial_ = {};
        }
    }
    Term(const MonomialT& monomial) : Term(1, monomial) {
    }
    Term(MonomialT&& monomial) : Term(1, std::move(monomial)) {
    }

    Coefficient Coeff() const {
        return coeff_;
    }
    const MonomialT& Monomial() const {
        return monomial_;
    }

    Term operator-() const {
        return Term(-coeff_, monomial_);
    }
    Term& operator*=(const Term& other) {
        coeff_ *= other.coeff_;
        monomial_ *= other.monomial_;

        if (coeff_ == 0) {
            monomial_ = {};
        }
        return *this;
    }
    Term& operator/=(const Term& other) {
        coeff_ /= other.coeff_;
        monomial_ /= other.monomial_;
        return *this;
    }

    friend Term operator*(const Term& lhs, const Term& rhs) {
        Term tmp(lhs);
        tmp *= rhs;
        return tmp;
    }
    friend Term operator/(const Term& lhs, const Term& rhs) {
        Term tmp(lhs);
        tmp /= rhs;
        return tmp;
    }

    friend bool operator==(const Term& lhs, const Term& rhs) = default;

private:
    Coefficient coeff_{};
    MonomialT monomial_;
};
}  // namespace gb
