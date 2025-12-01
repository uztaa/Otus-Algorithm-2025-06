#pragma once

#include "ILinearSorter.hpp"
#include <vector>
#include <cstdint>

/**
 * @brief Блочная сортировка (Bucket Sort)
 * 
 * Алгоритм:
 * 1. Распределяем элементы по корзинам
 * 2. Сортируем каждую корзину (std::sort)
 * 3. Объединяем корзины
 * 
 * Сложность: O(n + k) средняя, O(n²) худшая
 * Память: O(n + k)
 * Стабильность: Зависит от внутренней сортировки
 */
class BucketSorter : public ILinearSorter {
public:
    /**
     * @brief Конструктор
     * @param bucketCount Количество корзин (0 = автоматически)
     */
    explicit BucketSorter(size_t bucketCount = 0);
    ~BucketSorter() override = default;

    /**
     * @brief Сортировка 16-битных чисел
     */
    void sort(std::vector<uint16_t>& arr) override;

    /**
     * @brief Сортировка 32-битных чисел
     */
    void sort(std::vector<uint32_t>& arr, uint32_t maxValue = 0) override;

    std::string getName() const override { return "BucketSorter"; }

    /**
     * @brief Установить количество корзин
     */
    void setBucketCount(size_t count) { bucketCount_ = count; }

private:
    size_t bucketCount_;

    /**
     * @brief Шаблонная реализация BucketSort
     */
    template<typename T>
    void bucketSort(std::vector<T>& arr, T maxValue);
};