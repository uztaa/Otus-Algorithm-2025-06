#include "Sortable.h"

/** 
 * @file InsertionSorter.cpp
 * @brief Implementation of the Insertion Sort algorithm.
 */
class InsertionSorter : public Sortable {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;
        for (size_t i = 1; i < arr.size(); ++i) {
            size_t j = i;
			// Move the current element to its correct position in the sorted part of the array
            while (j > 0 && ++comparisons && arr[j].getKey() < arr[j - 1].getKey()) {
                std::swap(arr[j], arr[j - 1]);
                ++swaps;
                --j;
            }
        }
        return { comparisons, swaps };
    }
};