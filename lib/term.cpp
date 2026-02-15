#include "term.h"

namespace gb {
Term::Term(Coeff coeff) : coeff_(coeff) {
}

Term::Term(Coeff coeff, const Monomial& monomial) : coeff_(coeff), monomial_(monomial) {
}

Term::Term(Coeff coeff, Monomial&& monomial) : coeff_(coeff), monomial_(std::move(monomial)) {
}

Term::Term(const Monomial& monomial) : Term(1, monomial) {
}

Term::Term(Monomial&& monomial) : Term(1, std::move(monomial)) {
}

Term::Coeff Term::GetCoeff() const {
    return coeff_;
}

const Monomial& Term::GetMonomial() const {
    return monomial_;
}

Term Term::operator-() const {
    return Term(-coeff_, monomial_);
}

Term& Term::AddCoeff(Coeff other) {
    coeff_ += other;
    return *this;
}

Term& Term::operator*=(const Term& other) {
    coeff_ *= other.coeff_;
    monomial_ *= other.monomial_;
    return *this;
}

Term& Term::operator/=(const Term& other) {
    coeff_ /= other.coeff_;
    monomial_ /= other.monomial_;
    return *this;
}

Term operator*(const Term& lhs, const Term& rhs) {
    Term tmp(lhs);
    tmp *= rhs;
    return tmp;
}

Term operator/(const Term& lhs, const Term& rhs) {
    Term tmp(lhs);
    tmp /= rhs;
    return tmp;
}

bool operator==(const Term& lhs, const Term& rhs) {
    return (lhs.coeff_ == rhs.coeff_ && lhs.monomial_ == rhs.monomial_) ||
           (lhs.coeff_ == 0 && rhs.coeff_ == 0);
}

bool operator!=(const Term& lhs, const Term& rhs) {
    return !(lhs == rhs);
}

}  // namespace gb
