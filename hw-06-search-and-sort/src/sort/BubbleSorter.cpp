#include "Sortable.h"
#include <algorithm>

/**
 * @file BubbleSorter.cpp
 * @brief Реализация класса BubbleSorter для сортировки пузырьком.
 *
 * Этот класс реализует алгоритм сортировки пузырьком, который сравнивает пары соседних элементов
 * и меняет их местами, если они расположены в неправильном порядке.
 */
class BubbleSorter : public Sortable {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;
        for (size_t i = 0; i < arr.size(); ++i) {
            for (size_t j = arr.size() - 1; j > i; --j) {
                ++comparisons;
				// Если текущий элемент больше следующего, меняем их местами
                if (arr[j].getKey() < arr[j - 1].getKey()) {
                    std::swap(arr[j], arr[j - 1]);
                    ++swaps;
                }
            }
        }
        return {comparisons, swaps};
    }
};
