#include "BaseSorter.h"

/** 
 * @file InsertionSorter.cpp
 * @brief Реализация класса InsertionSorter для сортировки вставками.
 *
 * Этот класс реализует алгоритм сортировки вставками, который последовательно проходит по массиву
 * и вставляет каждый элемент на его правильное место в отсортированной части массива.
 */
class InsertionSorter : public BaseSorter {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;
        for (size_t i = 1; i < arr.size(); ++i) {
            size_t j = i;
			// Перемещение элементов влево, пока не найдём правильное место для текущего элемента
            while (j > 0 && ++comparisons && arr[j].getKey() < arr[j - 1].getKey()) {
                std::swap(arr[j], arr[j - 1]);
                ++swaps;
                --j;
            }
        }
        return { comparisons, swaps };
    }
};