#include <iostream>
#include "orderings.h"
#include "polynomial.h"
#include "rational.h"
#include "orderings.h"
#include "algo_utils.h"

int main() {
    using Algo = gb::Algorithm<gb::Rational, gb::LexOrder>;
    using Term = gb::Term<gb::Rational>;
    using Poly = Algo::Poly;

    // решаем домашку по алгебре (ДЗ6, задача 2)

    Poly f = Poly() + Term(1, {0, 4, 1}) - Term(1, {1, 1, 2}) + Term(1, {1, 2});
    Poly g = Poly() + Term(1, {0, 4, 5}) + Term(2, {1, 4, 1}) + Term(1, {2, 2});

    std::vector<Poly> set{std::move(f)};
    Algo::ReduceBy(set, &g);

    std::cout << "Остаток многочлена g относительно системы {f}:\n";
    std::cout << g << '\n';
}
