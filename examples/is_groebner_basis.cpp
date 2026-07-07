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

    // продолжаем решать домашку по алгебре (ДЗ6, задача 3)

    Poly f1 = Poly() + Term(2, {1, 1}) + Term(4, {1, 0, 1}) + Term(1, {0, 1, 2});
    Poly f2 = Poly() + Term(4, {1, 0, 2}) + Term(1, {0, 1, 3}) - Term(4);
    Poly f3 = Poly() + Term(1, {0, 2, 3}) - Term(4, {0, 1}) - Term(8, {0, 0, 1});

    std::vector<Poly> set{std::move(f1), std::move(f2), std::move(f3)};

    bool result = Algo::IsGroebnerBasis(set);
    if (result) {
        std::cout << "{f_1, f_2, f_3} является базисом Грёбнера\n";
    } else {
        std::cout << "{f_1, f_2, f_3} НЕ является базисом Грёбнера\n";
    }
}
