#pragma once
#include "Record.h"
#include <vector>
#include <random>

/**
 * @brief generator random values for test
 */
class RandomArrayGenerator
{
private:
    std::mt19937 rng_;

public:
    RandomArrayGenerator();
    explicit RandomArrayGenerator(uint32_t seed) : rng_(seed) {}

    void setSeed(uint32_t seed);

    /**
     * @brief generate random vector
     * @param size size of vector.
     * @return std::vector<Record>
     */
    std::vector<Record> generate(size_t size);                    // обычная генерация
    
    std::vector<Record> regenerate(size_t size, uint32_t seed);   // пересев + генерация
};