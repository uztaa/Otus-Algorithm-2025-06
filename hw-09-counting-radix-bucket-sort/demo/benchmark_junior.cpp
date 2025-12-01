#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <future>
#include <memory>

#include "CountingSorter.hpp"
#include "RadixSorter.hpp"
#include "BucketSorter.hpp"
#include "TimeFormatter.h"  // Из commonlib

constexpr uint32_t SEED = 12345u;
constexpr int64_t TIMEOUT_NS = 2LL * 60 * 1000 * 1000 * 1000; // 2 минуты
constexpr uint32_t MAX_VALUE = 999; // Диапазон 0-999

const std::vector<size_t> SIZES = {100, 1000, 10000, 100000, 1000000};

/**
 * @brief Генерация случайного массива
 */
std::vector<uint32_t> generateRandomArray(size_t size, uint32_t maxValue, uint32_t seed) {
    std::vector<uint32_t> arr;
    arr.reserve(size);
    
    std::mt19937 rng(seed);
    std::uniform_int_distribution<uint32_t> dist(0, maxValue);
    
    for (size_t i = 0; i < size; ++i) {
        arr.push_back(dist(rng));
    }
    
    return arr;
}

/**
 * @brief Сортировка с таймаутом
 */
std::pair<bool, int64_t> trySortWithTimeout(
    ILinearSorter& sorter, 
    std::vector<uint32_t>& data, 
    uint32_t maxValue,
    int64_t timeout_ns
) {
    auto start = std::chrono::steady_clock::now();

    auto fut = std::async(std::launch::async, [&sorter, &data, maxValue]() {
        sorter.sort(data, maxValue);
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
 * @brief Проверка отсортированности
 */
bool isSorted(const std::vector<uint32_t>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

int main() {
    TimeFormatter formatter;
    
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          Benchmark: Linear Sorting Algorithms (Junior Level)               ║\n";
    std::cout << "║                     Random array, values 0-999                             ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════════════════╝\n\n";

    std::cout << std::left
              << std::setw(12) << "Size"
              << std::setw(18) << "CountingSort"
              << std::setw(18) << "RadixSort"
              << std::setw(18) << "BucketSort"
              << std::setw(10) << "Verified" << '\n';
    std::cout << std::string(76, '=') << '\n';

    for (auto size : SIZES) {
        std::vector<uint32_t> baseData = generateRandomArray(size, MAX_VALUE, SEED);

        // CountingSort
        std::vector<uint32_t> countingData = baseData;
        CountingSorter countingSorter;
        auto [countingOk, countingTime] = trySortWithTimeout(
            countingSorter, countingData, MAX_VALUE, TIMEOUT_NS);
        bool countingVerified = countingOk && isSorted(countingData);

        // RadixSort
        std::vector<uint32_t> radixData = baseData;
        RadixSorter radixSorter;
        auto [radixOk, radixTime] = trySortWithTimeout(
            radixSorter, radixData, MAX_VALUE, TIMEOUT_NS);
        bool radixVerified = radixOk && isSorted(radixData);

        // BucketSort
        std::vector<uint32_t> bucketData = baseData;
        BucketSorter bucketSorter;
        auto [bucketOk, bucketTime] = trySortWithTimeout(
            bucketSorter, bucketData, MAX_VALUE, TIMEOUT_NS);
        bool bucketVerified = bucketOk && isSorted(bucketData);

        // Вывод результатов
        std::cout << std::left << std::setw(12) << size;
        
        if (countingOk) {
            std::cout << std::setw(18) << formatter.formatDuration(countingTime);
        } else {
            std::cout << std::setw(18) << "timeout";
        }
        
        if (radixOk) {
            std::cout << std::setw(18) << formatter.formatDuration(radixTime);
        } else {
            std::cout << std::setw(18) << "timeout";
        }
        
        if (bucketOk) {
            std::cout << std::setw(18) << formatter.formatDuration(bucketTime);
        } else {
            std::cout << std::setw(18) << "timeout";
        }

        std::string verified = (countingVerified && radixVerified && bucketVerified) ? "OK" : "FAIL";
        std::cout << std::setw(10) << verified << '\n';
    }

    std::cout << std::string(76, '=') << '\n';
    std::cout << "\nNote: Timeout = 2 minutes, Seed = " << SEED << "\n";

    return 0;
}