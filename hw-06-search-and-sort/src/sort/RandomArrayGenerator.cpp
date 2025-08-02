#include "RandomArrayGenerator.h"
#include <algorithm>
#include <chrono>

RandomArrayGenerator::RandomArrayGenerator() {
    // Инициализируем генератор случайных чисел текущим временем
    auto seed = static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count());
    rng_ = std::mt19937(seed);
}

std::vector<Record> RandomArrayGenerator::generate(size_t size) {
    std::vector<Record> result;
    result.reserve(size);

    std::uniform_int_distribution<int> dist(1, 100);
    for (size_t i = 0; i < size; ++i) {
        int randomKey = dist(rng_);
        result.emplace_back(randomKey, "x");
    }

    return result;
}

