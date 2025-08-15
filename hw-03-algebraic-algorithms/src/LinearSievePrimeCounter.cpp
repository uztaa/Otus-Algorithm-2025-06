#include "LinearSievePrimeCounter.h"
#include <vector>
#include <cstdint>

std::string LinearSievePrimeCounter::compute(const std::string& n_str) const {
    uint64_t n = std::stoull(n_str);
    if (n < 2) return "0";

    std::vector<uint64_t> lp(n + 1, 0);
    std::vector<uint64_t> primes;

    for (uint64_t i = 2; i <= n; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes.push_back(i);
        }
        for (uint64_t p : primes) {
            if (p > lp[i] || p * i > n) break;
            lp[p * i] = p;
        }
    }

    return std::to_string(primes.size());
}
