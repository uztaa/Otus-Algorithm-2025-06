#pragma once

#include <vector>
#include "Record.h"
#include "BaseSorter.h"

/**
 * @brief Имплементация сортировки вставками с использованием бинарного поиска.
 * @class BinaryInsertionSorter
 * @details
 * Реализует сортировку вставками с использованием бинарного поиска для нахождения позиции вставки.
 */
class BinaryInsertionSorter : public BaseSorter
{
public:
    /**
     * @brief Сортирует массив записей с использованием бинарного поиска.
     *
     * @param arr Массив записей для сортировки.
     * @return Пара, содержащая количество сравнений и сдвигов.
     */
    void sort(std::vector<Record> &arr) override;

protected:
    /**
     * @brief Выполняет бинарный поиск для нахождения позиции вставки.
     *
     * @param arr Массив записей, в котором выполняется поиск.
     * @param key Ключ, для которого ищется позиция вставки.
     * @param left Левая граница поиска.
     * @param right Правая граница поиска.
     * @return Индекс, где должен быть вставлен ключ.
     */
    size_t binarySearch(const std::vector<Record> &arr,
                        const Record &key,
                        size_t left,
                        size_t right);
};
