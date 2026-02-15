#include "monomial.h"
#include <cassert>

namespace gb {
Monomial::Monomial(std::initializer_list<Degree> degrees) : degrees_(degrees) {
}

size_t Monomial::GetSize() const {
    return degrees_.size();
}

Monomial::Degree Monomial::GetDegree(size_t index) const {
    if (index < degrees_.size()) {
        return degrees_[index];
    }
    return 0;
}

const std::vector<Monomial::Degree>& Monomial::GetDegrees() const {
    return degrees_;
}

bool Monomial::IsDivisibleBy(const Monomial& other) const {
    size_t max_size = std::max(degrees_.size(), other.degrees_.size());
    for (size_t i = 0; i < max_size; ++i) {
        if (GetDegree(i) < other.GetDegree(i)) {
            return false;
        }
    }
    return true;
}

Monomial& Monomial::operator*=(const Monomial& other) {
    if (degrees_.size() < other.degrees_.size()) {
        degrees_.resize(other.degrees_.size());
    }
    for (size_t i = 0; i < degrees_.size(); ++i) {
        degrees_[i] += other.GetDegree(i);
    }
    return *this;
}

Monomial& Monomial::operator/=(const Monomial& other) {
    assert(IsDivisibleBy(other));
    if (degrees_.size() < other.degrees_.size()) {
        degrees_.resize(other.degrees_.size());
    }
    for (size_t i = 0; i < degrees_.size(); ++i) {
        degrees_[i] -= other.GetDegree(i);
    }
    return *this;
}

Monomial operator*(const Monomial& lhs, const Monomial& rhs) {
    Monomial tmp(lhs);
    tmp *= rhs;
    return tmp;
}

Monomial operator/(const Monomial& lhs, const Monomial& rhs) {
    Monomial tmp(lhs);
    tmp /= rhs;
    return tmp;
}

bool operator==(const Monomial& lhs, const Monomial& rhs) {
    size_t max_size = std::max(lhs.degrees_.size(), rhs.degrees_.size());
    for (size_t i = 0; i < max_size; ++i) {
        if (lhs.GetDegree(i) != rhs.GetDegree(i)) {
            return false;
        }
    }
    return true;
}

bool operator!=(const Monomial& lhs, const Monomial& rhs) {
    return !(lhs == rhs);
}
}  // namespace gb
