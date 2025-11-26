#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>
#include <future>
#include <chrono>
#include <random>
#include <algorithm>
#include "SelectionSorter.h"
#include "HeapSorter.h"
#include "Record.h"
#include "TimeFormatter.h"

constexpr uint32_t SEED = 12345u;
constexpr int64_t TIMEOUT_NS = 2LL * 60 * 1000 * 1000 * 1000; // 2 minutes
const std::vector<size_t> SIZES = { 1, 10, 100, 1000, 10000, 100000, 1000000 };

enum class DataType {
    RANDOM,   // массив из случайных чисел
    DIGITS,   // массив из случайных цифр (0-9)
    SORTED,   // на 99% отсортированный массив
    REVERSED  // обратно-отсортированный массив
};

/**
 * @brief Generate test data based on type
 */
std::vector<Record> generateData(size_t size, DataType type, uint32_t seed) {
    std::vector<Record> result;
    result.reserve(size);
    
    std::mt19937 rng(seed);
    
    switch (type) {
        case DataType::RANDOM: {
            std::uniform_int_distribution<int> dist(1, 1000000);
            for (size_t i = 0; i < size; ++i) {
                result.emplace_back(dist(rng), "v" + std::to_string(i));
            }
            break;
        }
        
        case DataType::DIGITS: {
            std::uniform_int_distribution<int> dist(0, 9);
            for (size_t i = 0; i < size; ++i) {
                result.emplace_back(dist(rng), "v" + std::to_string(i));
            }
            break;
        }
        
        case DataType::SORTED: {
            // Create sorted array
            for (size_t i = 0; i < size; ++i) {
                result.emplace_back(static_cast<int>(i), "v" + std::to_string(i));
            }
            // Shuffle only 1% of elements
            size_t swapCount = std::max(size_t(1), size / 100);
            std::uniform_int_distribution<size_t> dist(0, size - 1);
            for (size_t i = 0; i < swapCount; ++i) {
                size_t idx1 = dist(rng);
                size_t idx2 = dist(rng);
                std::swap(result[idx1], result[idx2]);
            }
            break;
        }
        
        case DataType::REVERSED: {
            // Create reverse sorted array
            for (size_t i = 0; i < size; ++i) {
                result.emplace_back(static_cast<int>(size - i - 1), "v" + std::to_string(i));
            }
            break;
        }
    }
    
    return result;
}

std::string dataTypeToString(DataType type) {
    switch (type) {
        case DataType::RANDOM: return "random";
        case DataType::DIGITS: return "digits";
        case DataType::SORTED: return "sorted";
        case DataType::REVERSED: return "revers";
        default: return "unknown";
    }
}

/**
 * @brief Try to sort with timeout
 */
template<typename Sorter>
std::pair<bool, int64_t> trySortWithTimeout(Sorter& sorter, std::vector<Record>& data, int64_t timeout_ns) {
    auto start = std::chrono::steady_clock::now();

    auto fut = std::async(std::launch::async, [&sorter, &data]() {
        sorter.sort(data);
    });

    if (fut.wait_for(std::chrono::nanoseconds(timeout_ns)) == std::future_status::timeout) {
        return {false, -1};
    }

    fut.get();
    auto end = std::chrono::steady_clock::now();
    int64_t duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return {true, duration_ns};
}

/**
 * @brief Check if array is sorted
 */
bool isSorted(const std::vector<Record>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1].getKey() > arr[i].getKey()) {
            return false;
        }
    }
    return true;
}

void runExtendedBenchmark() {
    TimeFormatter formatter;
    
    std::vector<DataType> dataTypes = {
        DataType::RANDOM,
        DataType::DIGITS,
        DataType::SORTED,
        DataType::REVERSED
    };

    for (auto dataType : dataTypes) {
        std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║  Data Type: " << std::left << std::setw(45) 
                  << dataTypeToString(dataType) << "║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n\n";

        std::cout << std::left
                  << std::setw(12) << "Size"
                  << std::setw(22) << "SelectionSort"
                  << std::setw(22) << "HeapSort"
                  << std::setw(10) << "Verified" << '\n';
        std::cout << std::string(66, '=') << '\n';

        for (auto size : SIZES) {
            std::vector<Record> baseData = generateData(size, dataType, SEED);

            // SelectionSorter benchmark
            std::vector<Record> selectionData = baseData;
            SelectionSorter selectionSorter;
            auto [selSuccess, selDuration] = trySortWithTimeout(selectionSorter, selectionData, TIMEOUT_NS);
            bool selVerified = selSuccess && isSorted(selectionData);

            // HeapSorter benchmark
            std::vector<Record> heapData = baseData;
            HeapSorter heapSorter;
            auto [heapSuccess, heapDuration] = trySortWithTimeout(heapSorter, heapData, TIMEOUT_NS);
            bool heapVerified = heapSuccess && isSorted(heapData);

            // Print results
            std::cout << std::left << std::setw(12) << size;
            
            if (selSuccess) {
                std::cout << std::setw(22) << formatter.formatDuration(selDuration);
            } else {
                std::cout << std::setw(22) << "timeout";
            }
            
            if (heapSuccess) {
                std::cout << std::setw(22) << formatter.formatDuration(heapDuration);
            } else {
                std::cout << std::setw(22) << "timeout";
            }
            
            std::cout << std::setw(10) << (selVerified && heapVerified ? "✓" : "✗") << '\n';

            // Stop if Selection Sort timed out
            if (!selSuccess) {
                std::cout << "\nNote: Stopping this data type as Selection Sort timed out.\n";
                break;
            }
        }
        
        std::cout << std::string(66, '=') << '\n';
    }
    
    std::cout << "\n=== Summary ===\n";
    std::cout << "Timeout: 2 minutes\n";
    std::cout << "Selection Sort: O(n²) - Best for small arrays\n";
    std::cout << "Heap Sort: O(n log n) - Efficient for large arrays\n";
}

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║       Extended Benchmark: Selection Sort vs Heap Sort        ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    runExtendedBenchmark();
    return 0;
}