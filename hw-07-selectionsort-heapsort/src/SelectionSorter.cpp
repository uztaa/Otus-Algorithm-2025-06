#include "SelectionSorter.h"
#include "SortEvent.h"
#include <algorithm>

void SelectionSorter::sort(std::vector<Record>& arr) {
    if (arr.size() <= 1) {
        return;
    }
    
    notify(StartEvent(name));
    
    size_t n = arr.size();
    
    // One by one move boundary of unsorted subarray
    for (size_t i = 0; i < n - 1; ++i) {
        // Find the minimum element in unsorted array
        size_t minIdx = findMinIndex(arr, i, n);
        
        // Swap the found minimum element with the first element
        if (minIdx != i) {
            notify(SwapEvent(name, i, minIdx));
            std::swap(arr[i], arr[minIdx]);
        }
    }
    
    notify(FinishEvent(name));
}

size_t SelectionSorter::findMinIndex(const std::vector<Record>& arr, size_t start, size_t end) {
    size_t minIdx = start;
    
    for (size_t j = start + 1; j < end; ++j) {
        notify(CompareEvent(name, j, minIdx));
        
        if (arr[j].getKey() < arr[minIdx].getKey()) {
            minIdx = j;
        }
    }
    
    return minIdx;
}