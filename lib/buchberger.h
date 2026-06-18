#include <queue>
#include "algo_utils.h"
#include "polynomial.h"

namespace gb {
template <typename CoeffT, typename Ordering>
class Buchberger {
public:
    using Poly = Polynomial<CoeffT, Ordering>;
    using Set = std::vector<Poly>;
    using Algo = Algorithm<CoeffT, Ordering>;
    using Index = size_t;
    using Queue = std::queue<std::pair<Index, Index>>;

    static Set FindGroebnerBasis(Set set) {
        Queue queue = MakeIndexPairs(set.size());

        while (!queue.empty()) {
            auto [i, j] = queue.front();
            queue.pop();
            Polynomial s = Algo::SPolynomial(set[i], set[j]);
            Algo::ReduceBy(set, &s);
            if (!s.IsZero()) {
                queue = UpdateIndexPairs(std::move(queue), set.size());
                set.push_back(s);
            }
        }

        return set;
    }

private:
    static Queue MakeIndexPairs(size_t size) {
        Queue queue;
        for (Index i = 0; i < size; ++i) {
            for (Index j = i + 1; j < size; ++j) {
                queue.emplace(i, j);
            }
        }
        return queue;
    }

    static Queue UpdateIndexPairs(Queue queue, Index new_index) {
        for (Index k = 0; k != new_index; ++k) {
            queue.emplace(k, new_index);
        }

        return queue;
    }
};
}  // namespace gb
