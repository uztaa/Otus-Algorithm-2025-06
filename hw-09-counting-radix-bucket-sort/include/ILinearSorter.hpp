#pragma once

#include <vector>
#include <string>
#include <cstdint>

/**
 * @brief Интерфейс для алгоритмов линейной сортировки
 * 
 * Все алгоритмы работают с целыми числами без знака.
 * Поддерживают сортировку как в памяти, так и потенциально из файла.
 */
class ILinearSorter {
public:
    virtual ~ILinearSorter() = default;

    /**
     * @brief Сортировка массива 16-битных чисел
     * @param arr Массив для сортировки (in-place результат)
     */
    virtual void sort(std::vector<uint16_t>& arr) = 0;

    /**
     * @brief Сортировка массива 32-битных чисел
     * @param arr Массив для сортировки (in-place результат)
     * @param maxValue Максимальное значение в массиве (для оптимизации)
     */
    virtual void sort(std::vector<uint32_t>& arr, uint32_t maxValue = 0) = 0;

    /**
     * @brief Получить имя алгоритма
     * @return Название алгоритма
     */
    virtual std::string getName() const = 0;
};