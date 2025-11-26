#pragma once

#include "BaseSorter.h"
#include "Record.h"
#include <vector>

/**
 * @file SelectionSorter.h
 * @brief Implementation of Selection Sort algorithm
 * 
 * Selection Sort works by repeatedly finding the minimum element 
 * from unsorted part and putting it at the beginning.
 * 
 * Time Complexity:
 * - Best case: O(n^2)
 * - Average case: O(n^2)
 * - Worst case: O(n^2)
 * 
 * Space Complexity: O(1)
 * Stable: No (can be made stable with modifications)
 */
class SelectionSorter : public BaseSorter {
public:
    SelectionSorter() : BaseSorter("SelectionSorter") {}
    
    /**
     * @brief Sort array using Selection Sort algorithm
     * @param arr vector of Records to sort
     */
    void sort(std::vector<Record>& arr) override;

private:
    /**
     * @brief Find index of minimum element in range [start, end)
     * @param arr array to search
     * @param start starting index
     * @param end ending index (exclusive)
     * @return index of minimum element
     */
    size_t findMinIndex(const std::vector<Record>& arr, size_t start, size_t end);
};