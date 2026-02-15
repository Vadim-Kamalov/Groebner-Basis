#pragma once

#include <cstdint>
#include <vector>

namespace gb {
class Monomial {
public:
    using Degree = uint32_t;

    Monomial() = default;
    Monomial(std::initializer_list<Degree> degrees);

    Monomial(const Monomial&) = default;
    Monomial(Monomial&&) = default;
    Monomial& operator=(const Monomial&) = default;
    Monomial& operator=(Monomial&&) noexcept = default;
    ~Monomial() = default;

    size_t GetSize() const;
    Degree GetDegree(size_t index) const;
    const std::vector<Degree>& GetDegrees() const;
    bool IsDivisibleBy(const Monomial& other) const;

    Monomial& operator*=(const Monomial& other);
    Monomial& operator/=(const Monomial& other);

    friend Monomial operator*(const Monomial& lhs, const Monomial& rhs);
    friend Monomial operator/(const Monomial& lhs, const Monomial& rhs);

    friend bool operator==(const Monomial& lhs, const Monomial& rhs);
    friend bool operator!=(const Monomial& lhs, const Monomial& rhs);

private:
    std::vector<Degree> degrees_;
};
}  // namespace gb
