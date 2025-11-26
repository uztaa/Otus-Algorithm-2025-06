#include "HeapSorter.h"
#include "SortEvent.h"
#include <algorithm>

void HeapSorter::sort(std::vector<Record>& arr) {
    if (arr.size() <= 1) {
        return;
    }
    
    notify(StartEvent(name));
    
    size_t n = arr.size();
    
    // Build max heap
    buildMaxHeap(arr);
    
    // One by one extract elements from heap
    for (size_t i = n - 1; i > 0; --i) {
        // Move current root to end
        notify(SwapEvent(name, 0, i));
        std::swap(arr[0], arr[i]);
        
        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }
    
    notify(FinishEvent(name));
}

void HeapSorter::heapify(std::vector<Record>& arr, size_t n, size_t i) {
    size_t largest = i;       // Initialize largest as root
    size_t left = 2 * i + 1;  // left child
    size_t right = 2 * i + 2; // right child
    
    // If left child is larger than root
    if (left < n) {
        notify(CompareEvent(name, left, largest));
        if (arr[left].getKey() > arr[largest].getKey()) {
            largest = left;
        }
    }
    
    // If right child is larger than largest so far
    if (right < n) {
        notify(CompareEvent(name, right, largest));
        if (arr[right].getKey() > arr[largest].getKey()) {
            largest = right;
        }
    }
    
    // If largest is not root
    if (largest != i) {
        notify(SwapEvent(name, i, largest));
        std::swap(arr[i], arr[largest]);
        
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

void HeapSorter::buildMaxHeap(std::vector<Record>& arr) {
    size_t n = arr.size();
    
    // Build heap (rearrange array)
    // Start from last non-leaf node and heapify each node
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }
}