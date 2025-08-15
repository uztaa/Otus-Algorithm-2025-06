#include "SievePrimeCounter.h"
#include <vector>
#include <cstdint>

std::string SievePrimeCounter::compute(const std::string& n_str) const {
    uint64_t n = std::stoull(n_str);
    if (n < 2) return "0";

    // создаем решето и отфильтровываем простые числа
    std::vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;

    for (uint64_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }

    // подсчитываем простые числа
    uint64_t count = 0;
    for (bool p : is_prime)
        if (p) ++count;

    return std::to_string(count);
}