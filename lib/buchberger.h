#include <queue>
#include "algo_utils.h"
#include "polynomial.h"

namespace gb {
template <typename Ordering>
void FindGroebnerBasis(std::vector<Polynomial<Ordering>>& system) {
    std::queue<std::pair<int, int>> queue;
    for (size_t i = 0; i < system.size(); ++i) {
        for (size_t j = i + 1; j < system.size(); ++j) {
            queue.push({i, j});
        }
    }

    while (!queue.empty()) {
        auto [i, j] = queue.front();
        queue.pop();
        Polynomial<Ordering> s = CalcSPolynomial(system[i], system[j]);
        ReducePolynomialBySystem(s, system);
        if (!s.IsZero()) {
            for (size_t k = 0; k < system.size(); ++k) {
                queue.push({k, system.size()});
            }
            system.push_back(s);
        }
    }
}
}  // namespace gb
