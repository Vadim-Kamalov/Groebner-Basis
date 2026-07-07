#include <iostream>
#include "orderings.h"
#include "polynomial.h"
#include "rational.h"
#include "orderings.h"
#include "algo_utils.h"
#include "buchberger.h"

int main() {
    using Algo = gb::Algorithm<gb::Rational, gb::LexOrder>;
    using Buchberger = gb::Buchberger<gb::Rational, gb::LexOrder>;
    using Term = gb::Term<gb::Rational>;
    using Poly = Algo::Poly;

    // решаем домашку по алгебре (ДЗ7, задача 2)

    Poly f1 = Poly() + Term(1, {0, 3}) - Term(3, {1, 1});
    Poly f2 = Poly() + Term(1, {1, 2}) - Term(2, {2}) + Term(1, {0, 1});
    Poly f3 = Poly() + Term(1, {2, 1}) + Term(1, {0, 2});

    std::vector<Poly> set{std::move(f1), std::move(f2), std::move(f3)};

    set = Buchberger::FindGroebnerBasis(std::move(set));
    set = Algo::ReduceBasis(std::move(set));

    std::cout << "Минимальный редуцированный базис Грёбнера идеала (f_1, f_2, f_3):\n";
    for (const Poly& p : set) {
        std::cout << p << '\n';
    }
}
