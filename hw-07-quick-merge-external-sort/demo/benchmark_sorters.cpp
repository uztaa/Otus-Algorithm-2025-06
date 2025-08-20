#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>
#include <future>
#include <chrono>
#include "QuickSorter.h"
#include "MergeSorter.h"
#include "RandomArrayGenerator.h"
#include "DurationSortListener.h"
#include "PrintUtils.h"

constexpr uint32_t SEED = 12345u;
constexpr int TIMEOUT_MS = 2 * 60 * 1000; // таймаут в мс
const std::vector<size_t> SIZES = { 100, 1000, 10000, 100000, 1000000 };

template<typename Sorter>
bool trySortWithTimeout(Sorter& sorter, std::vector<Record>& data, int timeout_ms, long& duration_ms) {
    auto start = std::chrono::steady_clock::now();

    auto fut = std::async(std::launch::async, [&sorter, &data]() {
        sorter.sort(data);
    });

    if (fut.wait_for(std::chrono::milliseconds(timeout_ms)) == std::future_status::timeout) {
        return false; // таймаут
    }

    auto end = std::chrono::steady_clock::now();
    duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return true;
}

void runBenchmark() {
    RandomArrayGenerator generator;

    std::cout << std::setw(10) << "Size"
              << std::setw(15) << "QuickSort(ms)"
              << std::setw(15) << "MergeSort(ms)" << '\n';
    std::cout << std::string(40, '=') << '\n';

    for (auto size : SIZES) {
        std::vector<Record> data = generator.regenerate(size, SEED);

        // QuickSorter benchmark
        std::vector<Record> quickData = data;
        QuickSorter quickSorter;
        auto quickDurationListener = std::make_shared<DurationSortListener>();
        quickSorter.addListener(quickDurationListener);

        long quickDuration = 0;
        bool quickSuccess = trySortWithTimeout(quickSorter, quickData, TIMEOUT_MS, quickDuration);

        if (!quickSuccess) {
            std::cout << std::setw(10) << size 
                      << std::setw(15) << "timeout"
                      << std::setw(15) << "-" << '\n';
            break; // Прерываем бенчмарк по таймауту
        }

        // MergeSorter benchmark
        std::vector<Record> mergeData = data;
        MergeSorter mergeSorter;
        auto mergeDurationListener = std::make_shared<DurationSortListener>();
        mergeSorter.addListener(mergeDurationListener);

        long mergeDuration = 0;
        bool mergeSuccess = trySortWithTimeout(mergeSorter, mergeData, TIMEOUT_MS, mergeDuration);

        if (!mergeSuccess) {
            std::cout << std::setw(10) << size 
                      << std::setw(15) << quickDuration
                      << std::setw(15) << "timeout" << '\n';
            break; // Прерываем бенчмарк по таймауту
        }

        std::cout << std::setw(10) << size 
                  << std::setw(15) << quickDuration 
                  << std::setw(15) << mergeDuration << '\n';
    }
}

int main() {
    runBenchmark();
    return 0;
}
