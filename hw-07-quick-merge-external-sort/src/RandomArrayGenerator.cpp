#include "RandomArrayGenerator.h"
#include <algorithm>
#include <chrono>

#define MIN_DISTRIBUTION 1
#define MAX_DISTRIBUTION 1000000000

RandomArrayGenerator::RandomArrayGenerator()
{
    // по умолчанию сидим по времени
    auto seed = static_cast<uint32_t>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    rng_ = std::mt19937(seed);
}

void RandomArrayGenerator::setSeed(uint32_t seed) { 
    rng_.seed(seed); 
}

std::vector<Record> RandomArrayGenerator::generate(size_t size)
{
    std::vector<Record> result;
    result.reserve(size);

    std::uniform_int_distribution<int> dist(MIN_DISTRIBUTION, MAX_DISTRIBUTION);
    for (size_t i = 0; i < size; ++i)
    {
        int randomKey = dist(rng_);
        result.emplace_back(randomKey, ""); // пустая строка как value
    }

    return result;
}

std::vector<Record> RandomArrayGenerator::regenerate(size_t size, uint32_t seed) {
    setSeed(seed);
    return generate(size);
}