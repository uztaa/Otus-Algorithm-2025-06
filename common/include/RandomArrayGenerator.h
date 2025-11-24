#pragma once
#include "Record.h"
#include <vector>
#include <random>
#include <limits>


/**
 * @brief Генератор массивов Record
 */
class RandomArrayGenerator
{
private:
    std::mt19937 rng_;
    // значения по умолчанию для минимума и максимума распределения при генерации случайных чисел
    int min_dist;
    int max_dist;

public:
    RandomArrayGenerator();
    explicit RandomArrayGenerator(
        uint32_t seed, 
        int min_dist = 1,
        int max_dist = std::numeric_limits<int>::max()
    ) : 
    rng_(seed), min_dist(min_dist), max_dist(max_dist) {}

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