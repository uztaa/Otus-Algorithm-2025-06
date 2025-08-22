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

    /**
     * @brief Задать seed для генерации.
     */
    void setSeed(uint32_t seed);

    /**
     * @brief Генерируем вектор псевдо-случайных данных.
     * Используется для генерирования уникальных, не повторяющихся массивов.
     * @param size размер выходного вектора
     * @return std::vector<Record>
     */
    std::vector<Record> generate(size_t size);

    /**
     * @brief Задаем seed и генерируем вектор псевдо-случайных данных.
     * Используется для многократного генерирования одних и тех же массивов.
     * @param size размер выходного вектора
     * @param seed задаем seed
     * @return std::vector<Record>
     */
    std::vector<Record> regenerate(size_t size, uint32_t seed);
};