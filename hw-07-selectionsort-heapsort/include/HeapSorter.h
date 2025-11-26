#pragma once

#include "BaseSorter.h"
#include "Record.h"
#include <vector>

/**
 * @file HeapSorter.h
 * @brief Implementation of Heap Sort algorithm
 * 
 * Heap Sort is a comparison-based sorting algorithm that uses 
 * a binary heap data structure. It divides its input into 
 * a sorted and an unsorted region, and iteratively shrinks 
 * the unsorted region by extracting the largest element.
 * 
 * Time Complexity:
 * - Best case: O(n log n)
 * - Average case: O(n log n)
 * - Worst case: O(n log n)
 * 
 * Space Complexity: O(1)
 * Stable: No
 */
class HeapSorter : public BaseSorter {
public:
    HeapSorter() : BaseSorter("HeapSorter") {}
    
    /**
     * @brief Sort array using Heap Sort algorithm
     * @param arr vector of Records to sort
     */
    void sort(std::vector<Record>& arr) override;

private:
    /**
     * @brief Heapify a subtree rooted at index i
     * @param arr array to heapify
     * @param n size of heap
     * @param i root index of subtree
     */
    void heapify(std::vector<Record>& arr, size_t n, size_t i);
    
    /**
     * @brief Build a max heap from array
     * @param arr array to build heap from
     */
    void buildMaxHeap(std::vector<Record>& arr);
};