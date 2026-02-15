#include <cassert>
#include "algo_utils.h"

namespace gb {
Term LCM(const Term& lhs, const Term& rhs) {
    assert(lhs.GetCoeff() != 0);
    assert(rhs.GetCoeff() != 0);
    size_t size = std::max(lhs.GetMonomial().GetSize(), rhs.GetMonomial().GetSize());
    std::vector<Monomial::Degree> degrees(size);
    for (size_t i = 0; i < size; ++i) {
        degrees[i] = std::max(lhs.GetMonomial().GetDegree(i), rhs.GetMonomial().GetDegree(i));
    }
    Term result(1, Monomial(degrees));
    return result;
}
}  // namespace gb
