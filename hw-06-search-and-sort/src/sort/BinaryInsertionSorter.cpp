#include "BinaryInsertionSorter.h"
#include <iostream>

/**
 * @file BinaryInsertionSorter.cpp
 * @brief Реализация класса BinaryInsertionSorter для сортировки с использованием бинарного поиска.
 */

size_t BinaryInsertionSorter::binarySearch(const std::vector<Record>& arr,
    const Record& key,
    size_t left,
    size_t right,
    size_t& comparisons)
{
    while (left <= right) {
        // Вычисление среднего индекса
        size_t mid = (left + right) / 2;
        
        ++comparisons;

        if (arr[mid].getKey() < key.getKey()) {
			// Если ключ меньше, продолжаем поиск в правой части
            left = mid + 1;
        } 
        else {
            // Проверка выхода за границы массива
            if (mid == 0) break;

			// Если ключ больше или равен текущему элементу, продолжаем поиск в левой части
            right = mid - 1;
        }       
    }

    return left;
}

std::pair<size_t, size_t> BinaryInsertionSorter::sort(std::vector<Record>& arr) {
    size_t comparisons = 0;
    size_t shifts = 0;

    for (size_t i = 1; i < arr.size(); ++i) {
        Record current_record = arr[i];
        
		// Поиск позиции для вставки текущего ключа
        size_t pos = binarySearch(arr, current_record, 0, i - 1, comparisons);

        // FIXME: Лог для отладки
        /*
        std::cout << "Into array: ";
        for (const auto& rec : arr) {
            std::cout << "(" << rec.getKey() << ", " << rec.getValue() << ") ";
        }
        std::cout << "— find position " << pos
            << " for insert element (" << current_record.getKey() << ", " << current_record.getValue() << ")" << std::endl;
        */

        // Если найденная позиция — текущая, не делаем вставку
        if (pos == i) {
			// FIXME: Лог для отладки
			//std::cout << "Position is equal to current index, no need to shift elements." << std::endl;
            continue;
        };

		// Если позиция равна текущему индексу, ничего не делаем
		// иначе сдвигаем элементы вправо
        for (size_t j = i; j > pos; --j) {
            arr[j] = arr[j - 1];
            ++shifts;
        }

		// Вставляем ключ на найденную позицию
        arr[pos] = current_record;
    }

    return { comparisons, shifts };
}
