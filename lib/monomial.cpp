#include "monomial.h"
#include <cassert>
#include <numeric>

namespace gb {
Monomial::Monomial(std::initializer_list<Degree> degrees) : degrees_(degrees) {
    Normalize();
    degree_sum_ = std::accumulate(degrees_.begin(), degrees_.end(), Degree{0});
}
Monomial::Monomial(const std::vector<Degree>& degrees) : degrees_(degrees) {
    Normalize();
    degree_sum_ = std::accumulate(degrees_.begin(), degrees_.end(), Degree{0});
}
Monomial::Monomial(std::vector<Degree>&& degrees) : degrees_(std::move(degrees)) {
    Normalize();
    degree_sum_ = std::accumulate(degrees_.begin(), degrees_.end(), Degree{0});
}

Monomial::Degree Monomial::DegreeSum() const {
    return degree_sum_;
}

Monomial::Degree Monomial::DegreeOf(size_t index) const {
    if (index < degrees_.size()) {
        return degrees_[index];
    }
    return 0;
}

const std::vector<Monomial::Degree>& Monomial::Degrees() const {
    return degrees_;
}

bool Monomial::IsDivisibleBy(const Monomial& other) const {
    if (degrees_.size() < other.degrees_.size()) {
        return false;
    }
    for (size_t i = 0; i < other.degrees_.size(); ++i) {
        if (degrees_[i] < other.degrees_[i]) {
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
        degrees_[i] += other.DegreeOf(i);
        degree_sum_ += other.DegreeOf(i);
    }
    return *this;
}

Monomial& Monomial::operator/=(const Monomial& other) {
    assert(IsDivisibleBy(other));
    for (size_t i = 0; i < other.degrees_.size(); ++i) {
        degrees_[i] -= other.DegreeOf(i);
        degree_sum_ -= other.DegreeOf(i);
    }
    Normalize();
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

void Monomial::Normalize() {
    if (degrees_.empty()) {
        return;
    }

    size_t new_size = degrees_.size();
    while (new_size > 0 && degrees_[new_size - 1] == 0) {
        --new_size;
    }

    degrees_.resize(new_size);
}

}  // namespace gb
