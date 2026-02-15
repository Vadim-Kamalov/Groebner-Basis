#include "orderings.h"
#include <cassert>
#include <numeric>

namespace gb {
bool LexOrder::operator()(const Monomial& lhs, const Monomial& rhs) const {
    size_t max_size = std::max(lhs.GetSize(), rhs.GetSize());
    for (size_t i = 0; i < max_size; ++i) {
        if (lhs.GetDegree(i) == rhs.GetDegree(i)) {
            continue;
        }
        return lhs.GetDegree(i) > rhs.GetDegree(i);
    }
    return false;
}

bool LexOrder::operator()(const Term& lhs, const Term& rhs) const {
    assert(lhs.GetCoeff() != 0);
    assert(rhs.GetCoeff() != 0);
    return LexOrder::operator()(lhs.GetMonomial(), rhs.GetMonomial());
}

bool GrlexOrder::operator()(const Monomial& lhs, const Monomial& rhs) const {
    Monomial::Degree lhs_sum = std::accumulate(lhs.GetDegrees().begin(), lhs.GetDegrees().end(), 0);
    Monomial::Degree rhs_sum = std::accumulate(rhs.GetDegrees().begin(), rhs.GetDegrees().end(), 0);

    return (lhs_sum > rhs_sum) || (lhs_sum == rhs_sum && LexOrder()(lhs, rhs));
}

bool GrlexOrder::operator()(const Term& lhs, const Term& rhs) const {
    assert(lhs.GetCoeff() != 0);
    assert(rhs.GetCoeff() != 0);
    return GrlexOrder::operator()(lhs.GetMonomial(), rhs.GetMonomial());
}

}  // namespace gb
