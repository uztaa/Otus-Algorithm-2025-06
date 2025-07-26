#include "sortable.h"
#include <algorithm>

/** 
 * @file BubbleSorter.cpp
 * @brief Implementation of the Bubble Sort algorithm.
 */
class BubbleSorter : public Sortable {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;
        for (size_t i = 0; i < arr.size(); ++i) {
            for (size_t j = arr.size() - 1; j > i; --j) {
                ++comparisons;
				// Compare adjacent elements and swap if they are out of order
                if (arr[j].getKey() < arr[j - 1].getKey()) {
                    std::swap(arr[j], arr[j - 1]);
                    ++swaps;
                }
            }
        }
        return {comparisons, swaps};
    }
};
