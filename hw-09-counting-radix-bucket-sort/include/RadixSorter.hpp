#pragma once

#include "ILinearSorter.hpp"
#include <vector>
#include <cstdint>

/**
 * @brief Поразрядная сортировка (Radix Sort, LSD)
 * 
 * Алгоритм:
 * 1. Сортируем по младшему разряду (используя CountingSort)
 * 2. Переходим к следующему разряду
 * 3. Повторяем для всех разрядов
 * 
 * Сложность: O(d * (n + base)), где d - число разрядов
 * Память: O(n + base)
 * Стабильность: Да
 */
class RadixSorter : public ILinearSorter {
public:
    /**
     * @brief Конструктор
     * @param base Основание системы счисления (по умолчанию 256 - побайтовая)
     */
    explicit RadixSorter(uint32_t base = 256);
    ~RadixSorter() override = default;

    /**
     * @brief Сортировка 16-битных чисел
     */
    void sort(std::vector<uint16_t>& arr) override;

    /**
     * @brief Сортировка 32-битных чисел
     */
    void sort(std::vector<uint32_t>& arr, uint32_t maxValue = 0) override;

    std::string getName() const override { return "RadixSorter"; }

    /**
     * @brief Установить базу для поразрядной сортировки
     */
    void setBase(uint32_t base) { base_ = base; }

private:
    uint32_t base_;

    /**
     * @brief Сортировка по одному разряду (CountingSort по digit)
     */
    template<typename T>
    void countingSortByDigit(std::vector<T>& arr, T exp);
};