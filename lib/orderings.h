#pragma once

#include "monomial.h"
#include "term.h"

namespace gb {
struct LexOrder {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
    bool operator()(const Term& lhs, const Term& rhs) const;
};

struct GrlexOrder {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
    bool operator()(const Term& lhs, const Term& rhs) const;
};

// TODO: implement
// struct GrevlexOrder {
//     bool operator()(const Monomial& lhs, const Monomial& rhs) const;
//     bool operator()(const Term& lhs, const Term& rhs) const;
// };
}  // namespace gb
