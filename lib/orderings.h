#pragma once

#include "monomial.h"
#include "term.h"

namespace gb {
struct LexOrder {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct GrlexOrder {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};

struct GrevlexOrder {
    bool operator()(const Monomial& lhs, const Monomial& rhs) const;
};
}  // namespace gb
