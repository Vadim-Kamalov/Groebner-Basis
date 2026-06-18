#include <cassert>
#include "algo_utils.h"

namespace gb {
Monomial LCM(const Monomial& lhs, const Monomial& rhs) {
    size_t size = std::max(lhs.Degrees().size(), rhs.Degrees().size());

    std::vector<Monomial::Degree> degrees(size);
    for (size_t i = 0; i < size; ++i) {
        degrees[i] = std::max(lhs.DegreeOf(i), rhs.DegreeOf(i));
    }

    return Monomial(std::move(degrees));
}
}  // namespace gb
