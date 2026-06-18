#pragma once

#include <cassert>
#include <map>
#include "monomial.h"
#include "term.h"
#include "orderings.h"

namespace gb {
template <typename CoeffT = Rational, typename Ordering = LexOrder>
class Polynomial {
public:
    using TermT = Term<CoeffT>;
    using TermsContainer = std::map<Monomial, CoeffT, Ordering>;

    class Iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = TermT;
        using iterator_category = TermsContainer::const_iterator::iterator_category;

        const gb::Monomial& Monomial() const {
            return it_->first;
        }

        CoeffT Coeff() const {
            return it_->second;
        }

        TermT operator*() const {
            return TermT(Coeff(), Monomial());
        }

        Iterator& operator++() {
            ++it_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(Iterator other) const {
            return it_ == other.it_;
        }

    private:
        friend class Polynomial;
        Iterator(TermsContainer::const_iterator it) : it_(it) {
        }

        TermsContainer::const_iterator it_;
    };

public:
    Polynomial() = default;
    Polynomial(const TermT& term) {
        if (term.Coeff() != 0) {
            terms_[term.Monomial()] = term.Coeff();
        }
    }

    Iterator Begin() const {
        return Iterator(terms_.begin());
    }

    Iterator End() const {
        return Iterator(terms_.end());
    }

    bool IsZero() const {
        return terms_.empty();
    }

    const Monomial& LeadingMonomial() const {
        assert(!terms_.empty());
        return terms_.begin()->first;
    }

    CoeffT LeadingCoeff() const {
        assert(!terms_.empty());
        return terms_.begin()->second;
    }

    TermT LeadingTerm() const {
        assert(!terms_.empty());

        auto it = terms_.begin();
        Monomial monomial = it->first;
        CoeffT coeff = it->second;

        return TermT(coeff, std::move(monomial));
    }

    void Normalize() {
        CoeffT coeff = LeadingCoeff();
        *this /= coeff;
    }

    Polynomial operator-() const {
        TermsContainer new_terms = terms_;
        for (auto& [monomial, coeff] : new_terms) {
            coeff = -coeff;
        }
        return std::move(new_terms);
    }

    Polynomial& operator+=(const TermT& other) {
        if (other.Coeff() == 0) {
            return *this;
        }
        auto it = terms_.find(other.Monomial());

        if (it != terms_.end()) {
            it->second += other.Coeff();
            if (it->second == 0) {
                terms_.erase(it);
            }
        } else {
            terms_.emplace(other.Monomial(), other.Coeff());
        }

        return *this;
    }
    Polynomial& operator-=(const TermT& other) {
        if (other.Coeff() == 0) {
            return *this;
        }
        auto it = terms_.find(other.Monomial());

        if (it != terms_.end()) {
            it->second -= other.Coeff();
            if (it->second == 0) {
                terms_.erase(it);
            }
        } else {
            terms_.emplace(other.Monomial(), -other.Coeff());
        }

        return *this;
    }
    Polynomial& operator*=(const TermT& other) {
        if (other.Coeff() == 0) {
            return *this = Polynomial();
        }
        TermsContainer new_terms;
        std::vector<typename TermsContainer::node_type> nodes;
        nodes.reserve(terms_.size());

        for (auto it = terms_.begin(); it != terms_.end();) {
            auto node = terms_.extract(it++);
            node.key() *= other.Monomial();
            node.mapped() *= other.Coeff();
            nodes.emplace_back(std::move(node));
        }

        for (auto& node : nodes) {
            auto hint = terms_.end();
            terms_.insert(hint, std::move(node));
        }

        return *this;
    }

    Polynomial& operator+=(const Polynomial& other) {
        if (this == &other) {
            return *this *= CoeffT(2);
        }

        for (const auto& [monomial, coeff] : other.terms_) {
            AddTerm(coeff, monomial);
        }
        return *this;
    }
    Polynomial& operator-=(const Polynomial& other) {
        if (this == &other) {
            return *this = Polynomial();
        }

        for (const auto& [monomial, coeff] : other.terms_) {
            AddTerm(-coeff, monomial);
        }
        return *this;
    }
    Polynomial& operator*=(const Polynomial& other) {
        Polynomial result;
        for (const auto& [monomial, coeff] : other.terms_) {
            result += *this * Term(coeff, monomial);
        }
        return *this = std::move(result);
    }
    Polynomial& operator*=(CoeffT coeff) {
        if (coeff == 0) {
            return *this = Polynomial();
        }

        for (auto& [monomial, coeff_] : terms_) {
            coeff_ *= coeff;
        }
        return *this;
    }
    Polynomial& operator/=(CoeffT coeff) {
        assert(coeff != 0);

        for (auto& [monomial, coeff_] : terms_) {
            coeff_ /= coeff;
        }
        return *this;
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

    friend Polynomial operator*(const Polynomial& lhs, const TermT& rhs) {
        Polynomial tmp(lhs);
        tmp *= rhs;
        return tmp;
    }
    friend Polynomial operator*(const TermT& lhs, const Polynomial& rhs) {
        return rhs * lhs;
    }
    friend Polynomial operator+(const Polynomial& lhs, const TermT& rhs) {
        Polynomial tmp(lhs);
        tmp += rhs;
        return tmp;
    }
    friend Polynomial operator+(const TermT& lhs, const Polynomial& rhs) {
        return rhs + lhs;
    }
    friend Polynomial operator-(const Polynomial& lhs, const TermT& rhs) {
        Polynomial tmp(lhs);
        tmp -= rhs;
        return tmp;
    }
    friend Polynomial operator-(const TermT& lhs, const Polynomial& rhs) {
        Polynomial tmp(-rhs);
        tmp += lhs;
        return tmp;
    }

    friend Polynomial operator*(const Polynomial& lhs, CoeffT rhs) {
        Polynomial tmp(lhs);
        tmp *= rhs;
        return tmp;
    }
    friend Polynomial operator*(CoeffT lhs, const Polynomial& rhs) {
        return rhs * lhs;
    }
    friend Polynomial operator/(const Polynomial& lhs, CoeffT rhs) {
        Polynomial tmp(lhs);
        tmp /= rhs;
        return tmp;
    }

private:
    Polynomial(TermsContainer&& terms) : terms_(std::move(terms)) {
    }

    Polynomial& AddTerm(CoeffT coeff, const Monomial& monomial) {
        if (coeff == 0) {
            return *this;
        }
        auto it = terms_.find(monomial);

        if (it != terms_.end()) {
            it->second += coeff;
            if (it->second == 0) {
                terms_.erase(it);
            }
        } else {
            terms_.emplace(monomial, coeff);
        }

        return *this;
    }

    TermsContainer terms_;
};

template <typename CoeffT, typename Ordering>
std::ostream& operator<<(std::ostream& os, const Polynomial<CoeffT, Ordering>& p) {
    if (p.IsZero()) {
        return os << '0';
    }

    for (auto it = p.Begin(); it != p.End(); ++it) {
        if (it != p.Begin()) {
            if (it.Coeff() < 0) {
                os << " - ";
            } else {
                os << " + ";
            }
        }
        if (it.Coeff() == 1) {
            os << it.Monomial();
            continue;
        }
        if (it.Coeff() == -1) {
            os << it.Monomial();
            continue;
        }

        if (it.Coeff() < 0) {
            os << -it.Coeff() << it.Monomial();
        } else {
            os << it.Coeff() << it.Monomial();
        }
    }

    return os;
}
}  // namespace gb
