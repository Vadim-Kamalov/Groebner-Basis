#pragma once

#include <cstdint>
#include <ostream>
#include <vector>

namespace gb {
class Monomial {
public:
    using Degree = uint32_t;

    Monomial() = default;
    Monomial(std::initializer_list<Degree> degrees);
    Monomial(const std::vector<Degree>& degrees);
    Monomial(std::vector<Degree>&& degrees);

    Degree DegreeSum() const;
    Degree DegreeOf(size_t index) const;
    const std::vector<Degree>& Degrees() const;
    bool IsDivisibleBy(const Monomial& other) const;

    Monomial& operator*=(const Monomial& other);
    Monomial& operator/=(const Monomial& other);

    friend Monomial operator*(const Monomial& lhs, const Monomial& rhs);
    friend Monomial operator/(const Monomial& lhs, const Monomial& rhs);

    friend bool operator==(const Monomial& lhs, const Monomial& rhs) = default;

private:
    void Normalize();

    std::vector<Degree> degrees_;
    Degree degree_sum_ = 0;
};

std::ostream& operator<<(std::ostream& os, const Monomial& m);
}  // namespace gb
