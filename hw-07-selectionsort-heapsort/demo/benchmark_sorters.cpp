#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>
#include <future>
#include <chrono>
#include "SelectionSorter.h"
#include "HeapSorter.h"
#include "RandomArrayGenerator.h"
#include "DurationSortListener.h"
#include "TimeFormatter.h"

constexpr uint32_t SEED = 12345u;
constexpr int64_t TIMEOUT_NS = 2LL * 60 * 1000 * 1000 * 1000; // 2 minutes in nanoseconds
const std::vector<size_t> SIZES = { 100, 1000, 10000, 100000, 1000000 };

/**
 * @brief Try to sort with timeout
 * @return pair<success, duration_ns>
 */
template<typename Sorter>
std::pair<bool, int64_t> trySortWithTimeout(Sorter& sorter, std::vector<Record>& data, int64_t timeout_ns) {
    auto start = std::chrono::steady_clock::now();

    auto fut = std::async(std::launch::async, [&sorter, &data]() {
        sorter.sort(data);
    });

    if (fut.wait_for(std::chrono::nanoseconds(timeout_ns)) == std::future_status::timeout) {
        return {false, -1}; // timeout
    }

    fut.get(); // ensure completion
    auto end = std::chrono::steady_clock::now();
    int64_t duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return {true, duration_ns};
}

void runBenchmark() {
    RandomArrayGenerator generator;
    TimeFormatter formatter;

    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║       Benchmark: Selection Sort vs Heap Sort            ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    std::cout << std::left
              << std::setw(15) << "Size"
              << std::setw(20) << "SelectionSort"
              << std::setw(20) << "HeapSort" << '\n';
    std::cout << std::string(55, '=') << '\n';

    for (auto size : SIZES) {
        std::vector<Record> baseData = generator.regenerate(size, SEED);

        // SelectionSorter benchmark
        std::vector<Record> selectionData = baseData;
        SelectionSorter selectionSorter;

        auto [selectionSuccess, selectionDuration] = trySortWithTimeout(selectionSorter, selectionData, TIMEOUT_NS);

        // HeapSorter benchmark
        std::vector<Record> heapData = baseData;
        HeapSorter heapSorter;

        auto [heapSuccess, heapDuration] = trySortWithTimeout(heapSorter, heapData, TIMEOUT_NS);

        // Print results
        std::cout << std::left << std::setw(15) << size;
        
        if (selectionSuccess) {
            std::cout << std::setw(20) << formatter.formatDuration(selectionDuration);
        } else {
            std::cout << std::setw(20) << "timeout";
        }
        
        if (heapSuccess) {
            std::cout << std::setw(20) << formatter.formatDuration(heapDuration);
        } else {
            std::cout << std::setw(20) << "timeout";
        }
        
        std::cout << '\n';

        // If Selection Sort timed out, likely no point continuing with larger sizes
        if (!selectionSuccess) {
            std::cout << "\nNote: Stopping benchmark as Selection Sort timed out.\n";
            break;
        }
    }
    
    std::cout << std::string(55, '=') << '\n';
    std::cout << "\nNote: Timeout is set to 2 minutes\n";
}

int main() {
    runBenchmark();
    return 0;
}