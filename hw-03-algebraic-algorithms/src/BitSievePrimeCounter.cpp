#include "BitSievePrimeCounter.h"
#include <vector>
#include <cstdint>

std::string BitSievePrimeCounter::compute(const std::string& n_str) const {
    uint64_t n = std::stoull(n_str);
    if (n < 2) return "0";

    uint64_t size = (n + 31) / 32;
    std::vector<uint32_t> sieve(size, 0xFFFFFFFF);

    sieve[0] &= ~3; // обнуляем 0 и 1

    for (uint64_t i = 2; i * i <= n; ++i) {
        if (sieve[i / 32] & (1u << (i % 32))) {
            for (uint64_t j = i * i; j <= n; j += i) {
                sieve[j / 32] &= ~(1u << (j % 32));
            }
        }
    }

    uint64_t count = 0;
    for (uint64_t i = 2; i <= n; ++i) {
        if (sieve[i / 32] & (1u << (i % 32))) ++count;
    }

    return std::to_string(count);
}
