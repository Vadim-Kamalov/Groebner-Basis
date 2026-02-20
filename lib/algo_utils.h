#pragma once

#include "term.h"
#include "polynomial.h"

namespace gb {
Term LCM(const Term& lhs, const Term& rhs);

template <typename Ordering>
Polynomial<Ordering> CalcSPolynomial(const Polynomial<Ordering>& f, const Polynomial<Ordering>& g) {
    Term lcm = LCM(f.GetLeadingTerm(), g.GetLeadingTerm());
    Polynomial s = (lcm / f.GetLeadingTerm()) * f - (lcm / g.GetLeadingTerm()) * g;
    return s;
}

template <typename Ordering>
void ReducePolynomialBySystem(Polynomial<Ordering>& to_reduce,
                              const std::vector<Polynomial<Ordering>>& system) {
    bool reduced = true;
    while (reduced) {
        reduced = false;
        for (const Polynomial<Ordering>& polynomial : system) {
            if (to_reduce.Reduce(polynomial)) {
                reduced = true;
            }
        }
    }
}

template <typename Ordering>
bool IsGroebnerBasis(const std::vector<Polynomial<Ordering>>& system) {
    for (size_t i = 0; i < system.size(); ++i) {
        for (size_t j = i + 1; j < system.size(); ++j) {
            Polynomial s = CalcSPolynomial(system[i], system[j]);
            ReducePolynomialBySystem(s, system);
            if (!s.IsZero()) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace gb
