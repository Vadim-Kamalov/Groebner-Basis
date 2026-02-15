#pragma once

#include <cassert>
#include <set>
#include "term.h"
#include "orderings.h"

namespace gb {
template <typename Ordering = LexOrder>
class Polynomial {
public:
    using TermsContainer = std::set<Term, Ordering>;

    Polynomial() = default;
    Polynomial(const Term& other) {
        if (other.GetCoeff() != 0) {
            terms_.insert(other);
        }
    }
    Polynomial(Term&& other) {
        if (other.GetCoeff() != 0) {
            terms_.insert(std::move(other));
        }
    }
    Polynomial(const TermsContainer& terms) {
        for (const Term& term : terms) {
            if (term.GetCoeff() != 0) {
                terms_.insert(term);
            }
        }
    }

    Polynomial(const Polynomial&) = default;
    Polynomial(Polynomial&&) = default;
    Polynomial& operator=(const Polynomial&) = default;
    Polynomial& operator=(Polynomial&&) noexcept = default;
    ~Polynomial() = default;

    const TermsContainer& GetTerms() const {
        return terms_;
    }
    const Term& GetLeadingTerm() const {
        assert(!terms_.empty());
        return *terms_.begin();
    }

    Polynomial operator-() const {
        TermsContainer new_terms;
        for (const Term& term : terms_) {
            new_terms.insert(-term);
        }
        return Polynomial(new_terms);
    }

    Polynomial& operator+=(const Term& other) {
        if (other.GetCoeff() == 0) {
            return *this;
        }
        for (const Term& term : terms_) {
            if (other.GetMonomial() == term.GetMonomial()) {
                Term new_term(term);
                new_term.AddCoeff(other.GetCoeff());
                terms_.erase(term);
                if (new_term.GetCoeff() != 0) {
                    terms_.insert(new_term);
                }
                return *this;
            }
        }
        terms_.insert(other);
        return *this;
    }
    Polynomial& operator-=(const Term& other) {
        return *this += -other;
    }
    Polynomial& operator*=(const Term& other) {
        Polynomial result;
        for (const Term& term : terms_) {
            result += term * other;
        }
        return *this = result;
    }

    Polynomial& operator+=(const Polynomial& other) {
        for (const Term& term : other.GetTerms()) {
            *this += term;
        }
        return *this;
    }
    Polynomial& operator-=(const Polynomial& other) {
        for (const Term& term : other.GetTerms()) {
            *this -= term;
        }
        return *this;
    }
    Polynomial& operator*=(const Polynomial& other) {
        Polynomial result;
        for (const Term& term : other.GetTerms()) {
            result += *this * term;
        }
        return *this = result;
    }

    friend bool operator==(const Polynomial& lhs, const Polynomial& rhs) {
        return lhs.terms_ == rhs.terms_;
    }

    friend bool operator!=(const Polynomial& lhs, const Polynomial& rhs) {
        return !(lhs == rhs);
    }

    friend Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial tmp(lhs);
        tmp += rhs;
        return tmp;
    }
    friend Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial tmp(lhs);
        tmp -= rhs;
        return tmp;
    }
    friend Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs) {
        Polynomial tmp(lhs);
        tmp *= rhs;
        return tmp;
    }

    friend Polynomial operator*(const Polynomial& lhs, const Term& rhs) {
        Polynomial tmp(lhs);
        tmp *= rhs;
        return tmp;
    }
    friend Polynomial operator*(const Term& lhs, const Polynomial& rhs) {
        Polynomial tmp(rhs);
        tmp *= lhs;
        return tmp;
    }

    bool Reduce(const Polynomial& other) {
        const Term& lead_term = other.GetLeadingTerm();
        for (const Term& term : terms_) {
            if (term.GetMonomial().IsDivisibleBy(lead_term.GetMonomial())) {
                *this -= (term / lead_term) * other;
                return true;
            }
        }
        return false;
    }

private:
    TermsContainer terms_;
};
}  // namespace gb
