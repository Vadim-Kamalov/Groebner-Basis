#include "orderings.h"
#include <cassert>
#include <numeric>

namespace gb {
bool LexOrder::operator()(const Monomial& lhs, const Monomial& rhs) const {
    size_t max_size = std::max(lhs.Degrees().size(), rhs.Degrees().size());
    for (size_t i = 0; i < max_size; ++i) {
        if (lhs.DegreeOf(i) != rhs.DegreeOf(i)) {
            return lhs.DegreeOf(i) > rhs.DegreeOf(i);
        }
    }
    return false;
}

bool GrlexOrder::operator()(const Monomial& lhs, const Monomial& rhs) const {
    Monomial::Degree lhs_sum = lhs.DegreeSum();
    Monomial::Degree rhs_sum = rhs.DegreeSum();

    return (lhs_sum > rhs_sum) || (lhs_sum == rhs_sum && LexOrder()(lhs, rhs));
}

bool GrevlexOrder::operator()(const Monomial& lhs, const Monomial& rhs) const {
    Monomial::Degree lhs_sum = lhs.DegreeSum();
    Monomial::Degree rhs_sum = rhs.DegreeSum();

    if (lhs_sum != rhs_sum) {
        return lhs_sum > rhs_sum;
    }

    size_t max_size = std::max(lhs.Degrees().size(), rhs.Degrees().size());
    for (size_t i = max_size; i > 0; --i) {
        if (lhs.DegreeOf(i - 1) != rhs.DegreeOf(i - 1)) {
            return lhs.DegreeOf(i - 1) < rhs.DegreeOf(i - 1);
        }
    }

    return false;
}

}  // namespace gb
