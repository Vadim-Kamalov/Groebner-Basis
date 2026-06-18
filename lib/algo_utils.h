#pragma once

#include <algorithm>
#include "monomial.h"
#include "polynomial.h"

namespace gb {

Monomial LCM(const Monomial& lhs, const Monomial& rhs);

template <typename CoeffT, typename Ordering>
class Algorithm {
public:
    using Polynomial = Polynomial<CoeffT, Ordering>;
    using Set = std::vector<Polynomial>;

    static Polynomial SPolynomial(const Polynomial& f, const Polynomial& g) {
        Term lcm = LCM(f.LeadingMonomial(), g.LeadingMonomial());
        Polynomial s = (lcm / f.LeadingTerm()) * f - (lcm / g.LeadingTerm()) * g;
        return s;
    }

    enum class Status : uint8_t { NoReduction, ReducedOnce };

    static Status ReduceOnce(const Polynomial& reduce_by, Polynomial* to_reduce) {
        assert(to_reduce != nullptr);
        if (reduce_by.IsZero()) {
            return Status::NoReduction;
        }
        const Monomial& lead_monomial = reduce_by.LeadingMonomial();
        for (auto it = to_reduce->Begin(); it != to_reduce->End(); ++it) {
            if (it.Monomial().IsDivisibleBy(lead_monomial)) {
                *to_reduce -= (*it / reduce_by.LeadingTerm()) * reduce_by;
                return Status::ReducedOnce;
            }
        }
        return Status::NoReduction;
    }

    template <typename Iterator>
    static Status ReduceOnceBy(Iterator begin, Iterator end, Polynomial* to_reduce) {
        assert(to_reduce != nullptr);
        for (Iterator it = begin; it != end; ++it) {
            if (ReduceOnce(*it, to_reduce) == Status::ReducedOnce) {
                return Status::ReducedOnce;
            }
        }
        return Status::NoReduction;
    }

    template <typename Iterator>
    static void ReduceBy(Iterator begin, Iterator end, Polynomial* to_reduce) {
        assert(to_reduce != nullptr);
        Status status = Status::NoReduction;
        do {
            status = ReduceOnceBy(begin, end, to_reduce);
        } while (status == Status::ReducedOnce);
    }

    static Status ReduceOnceBy(const Set& set, Polynomial* to_reduce) {
        return ReduceOnceBy(set.begin(), set.end(), to_reduce);
    }

    static void ReduceBy(const Set& set, Polynomial* to_reduce) {
        return ReduceBy(set.begin(), set.end(), to_reduce);
    }

    static bool IsGroebnerBasis(const Set& set) {
        for (size_t i = 0; i < set.size(); ++i) {
            for (size_t j = i + 1; j < set.size(); ++j) {
                Polynomial s = SPolynomial(set[i], set[j]);
                ReduceBy(set, &s);
                if (!s.IsZero()) {
                    return false;
                }
            }
        }
        return true;
    }

    template <typename Iterator>
    static void SortSetByMonomials(Iterator begin, Iterator end) {
        std::sort(begin, end, [](const Polynomial& left, const Polynomial& right) {
            return Ordering()(left.LeadingMonomial(), right.LeadingMonomial());
        });
    }

    static Set ReduceBasis(Set basis) {
        assert(IsGroebnerBasis(basis));

        for (auto& p : basis) {
            p.Normalize();
        }

        for (size_t i = 0; i < basis.size(); ++i) {
            ReduceByExcept(basis, i, &basis[i]);

            if (!basis[i].IsZero()) {
                basis[i].Normalize();
            }
        }

        basis.erase(std::remove_if(basis.begin(), basis.end(),
                                   [](const Polynomial& p) { return p.IsZero(); }),
                    basis.end());

        SortSetByMonomials(basis.begin(), basis.end());

        return basis;
    }

private:
    static Status ReduceOnceByExcept(const Set& set, size_t except, Polynomial* to_reduce) {
        assert(to_reduce != nullptr);
        for (size_t i = 0; i != set.size(); ++i) {
            if (i == except) {
                continue;
            }
            if (ReduceOnce(set[i], to_reduce) == Status::ReducedOnce) {
                return Status::ReducedOnce;
            }
        }
        return Status::NoReduction;
    }

    static void ReduceByExcept(const Set& set, size_t except, Polynomial* to_reduce) {
        assert(to_reduce != nullptr);
        Status status = Status::NoReduction;
        do {
            status = ReduceOnceByExcept(set, except, to_reduce);
        } while (status == Status::ReducedOnce);
    }
};

}  // namespace gb
