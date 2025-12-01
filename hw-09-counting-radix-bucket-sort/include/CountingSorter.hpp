#pragma once

#include "ILinearSorter.hpp"
#include <vector>
#include <cstdint>

/**
 * @brief Сортировка подсчётом (Counting Sort)
 * 
 * Алгоритм:
 * 1. Подсчитываем количество каждого значения
 * 2. Вычисляем кумулятивные суммы (позиции)
 * 3. Строим отсортированный массив
 * 
 * Сложность: O(n + k), где k - диапазон значений
 * Память: O(k) для счётчиков + O(n) для output
 * Стабильность: Да
 */
class CountingSorter : public ILinearSorter {
public:
    CountingSorter() = default;
    ~CountingSorter() override = default;

    /**
     * @brief Сортировка 16-битных чисел (0-65535)
     */
    void sort(std::vector<uint16_t>& arr) override;

    /**
     * @brief Сортировка 32-битных чисел с заданным максимумом
     * @param arr Массив для сортировки
     * @param maxValue Максимальное значение (если 0, вычисляется автоматически)
     */
    void sort(std::vector<uint32_t>& arr, uint32_t maxValue = 0) override;

    std::string getName() const override { return "CountingSorter"; }

private:
    /**
     * @brief Шаблонная реализация CountingSort
     */
    template<typename T>
    void countingSort(std::vector<T>& arr, T maxValue);
};