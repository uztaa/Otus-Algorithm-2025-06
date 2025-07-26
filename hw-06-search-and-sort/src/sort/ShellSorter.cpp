#include "Sortable.h"

/**
 * @file ShellSorter.cpp
 * @brief Имплементация класса ShellSorter для сортировки с использованием алгоритма Шелла.
 */
class ShellSorter : public Sortable {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;
		// Start with a large gap and reduce it until it becomes 0
        for (size_t gap = arr.size() / 2; gap > 0; gap /= 2) {
            for (size_t i = gap; i < arr.size(); ++i) {
                size_t j = i;
				// Perform a gapped insertion sort for this gap size
                while (j >= gap && ++comparisons && arr[j - gap].getKey() > arr[j].getKey()) {
                    std::swap(arr[j], arr[j - gap]);
                    ++swaps;
                    j -= gap;
                }
            }
        }
        return { comparisons, swaps };
    }
};