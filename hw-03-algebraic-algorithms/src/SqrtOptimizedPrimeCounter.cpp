#include "SqrtOptimizedPrimeCounter.h"
#include <cmath>
#include <cstdint>

std::string SqrtOptimizedPrimeCounter::compute(const std::string& n_str) const {
    uint64_t n = std::stoull(n_str);
    uint64_t count = 0;

    for (uint64_t i = 2; i <= n; ++i) {
        bool is_prime = true;
        for (uint64_t j = 2; j * j <= i; ++j) {
            if (i % j == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) ++count;
    }

    return std::to_string(count);
}
