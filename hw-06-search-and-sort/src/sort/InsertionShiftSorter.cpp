#include "Sortable.h"

/** 
 * @file InsertionShiftSorter.cpp
 * @brief Implementation of the Insertion Sort algorithm using shifts instead of swaps.
 */
class InsertionShiftSorter : public Sortable {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, shifts = 0;
        for (size_t i = 1; i < arr.size(); ++i) {
            Record temp = arr[i];
            size_t j = i;
			// Move the current element to its correct position in the sorted part of the array
            while (j > 0 && ++comparisons && arr[j - 1].getKey() > temp.getKey()) {
                arr[j] = arr[j - 1];
                ++shifts;
                --j;
            }
            arr[j] = temp;
        }
        return { comparisons, shifts };
    }
};