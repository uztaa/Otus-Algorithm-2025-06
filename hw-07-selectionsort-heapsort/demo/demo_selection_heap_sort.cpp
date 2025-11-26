#include <iostream>
#include <memory>
#include <vector>
#include "SelectionSorter.h"
#include "HeapSorter.h"
#include "RandomArrayGenerator.h"
#include "LoggingSortListener.h"
#include "Timer.h"
#include "TimeFormatter.h"
#include "PrintUtils.h"

constexpr uint32_t SEED = 12345u;

void demonstrateSorter(BaseSorter& sorter, std::vector<Record>& data) {
    std::cout << "\n========================================\n";
    std::cout << "Sorting with: " << sorter.getName() << "\n";
    std::cout << "========================================\n";
    
    std::cout << "Before sorting:\n";
    printRecords(data, std::cout);
    
    sorter.sort(data);
    
    std::cout << "\nAfter sorting:\n";
    printRecords(data, std::cout);
    std::cout << "\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << "   Selection Sort & Heap Sort Demo\n";
    std::cout << "===========================================\n";
    
    const size_t testSize = 10;
    
    // Test 1: Selection Sort
    {
        RandomArrayGenerator generator(42);  // свой generator с seed
        std::vector<Record> data = generator.generate(testSize);
        SelectionSorter selectionSorter;
        demonstrateSorter(selectionSorter, data);
    }
    
    // Test 2: Heap Sort
    {
        RandomArrayGenerator generator(42);  // тот же seed для сравнения
        std::vector<Record> data = generator.generate(testSize);
        HeapSorter heapSorter;
        demonstrateSorter(heapSorter, data);
    }
    
    // Test 3: Compare on the same data
    std::cout << "\n===========================================\n";
    std::cout << "   Performance Comparison\n";
    std::cout << "===========================================\n";
    
    const size_t perfTestSize = 1000;
    RandomArrayGenerator generator(777);
    std::vector<Record> baseData = generator.generate(perfTestSize);
    TimeFormatter formatter;
    
    // Selection Sort
    {
        std::vector<Record> data = baseData;
        SelectionSorter sorter;
        
        Timer timer;
        timer.start();
        sorter.sort(data);
        timer.stop();
        
        std::cout << "\nSelection Sort (n=" << perfTestSize << "): ";
        std::cout << formatter.formatDuration(timer.getDurationNs()) << "\n";
    }
    
    // Heap Sort
    {
        std::vector<Record> data = baseData;
        HeapSorter sorter;
        
        Timer timer;
        timer.start();
        sorter.sort(data);
        timer.stop();
        
        std::cout << "Heap Sort (n=" << perfTestSize << "): ";
        std::cout << formatter.formatDuration(timer.getDurationNs()) << "\n";
    }
    
    return 0;
}