#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>
#include <cstdint>

#include "CountingSorter.hpp"
#include "RadixSorter.hpp"
#include "BucketSorter.hpp"
#include "TimeFormatter.h"  // Из commonlib

/**
 * @brief Загрузка данных из бинарного файла
 */
std::vector<uint16_t> loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    size_t count = fileSize / sizeof(uint16_t);
    std::vector<uint16_t> data(count);

    if (!file.read(reinterpret_cast<char*>(data.data()), fileSize)) {
        throw std::runtime_error("Failed to read file: " + filename);
    }

    return data;
}

/**
 * @brief Проверка отсортированности
 */
bool isSorted(const std::vector<uint16_t>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

/**
 * @brief Запуск сортировки с измерением времени
 */
template<typename Sorter>
int64_t runSort(Sorter& sorter, std::vector<uint16_t>& data) {
    auto start = std::chrono::steady_clock::now();
    sorter.sort(data);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        std::cerr << "  input_file - binary file with 16-bit numbers\n";
        std::cerr << "\nExample:\n";
        std::cerr << "  " << argv[0] << " data.bin\n";
        return 1;
    }

    std::string inputFile = argv[1];
    TimeFormatter formatter;

    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          Benchmark: Linear Sorting Algorithms (Middle Level)               ║\n";
    std::cout << "║                     16-bit numbers from file (0-65535)                     ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════════════════╝\n\n";

    // Загрузка данных
    std::cout << "Loading data from '" << inputFile << "'...\n";
    auto loadStart = std::chrono::steady_clock::now();
    
    std::vector<uint16_t> baseData;
    try {
        baseData = loadFromFile(inputFile);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    auto loadEnd = std::chrono::steady_clock::now();
    int64_t loadTime = std::chrono::duration_cast<std::chrono::nanoseconds>(loadEnd - loadStart).count();
    
    std::cout << "Loaded " << baseData.size() << " numbers in " 
              << formatter.formatDuration(loadTime) << "\n\n";

    std::cout << std::left
              << std::setw(20) << "Algorithm"
              << std::setw(20) << "Time"
              << std::setw(15) << "Verified" << '\n';
    std::cout << std::string(55, '=') << '\n';

    // CountingSort
    {
        std::vector<uint16_t> data = baseData;
        CountingSorter sorter;
        int64_t time = runSort(sorter, data);
        bool verified = isSorted(data);
        
        std::cout << std::setw(20) << sorter.getName()
                  << std::setw(20) << formatter.formatDuration(time)
                  << std::setw(15) << (verified ? "OK" : "FAIL") << '\n';
    }

    // RadixSort
    {
        std::vector<uint16_t> data = baseData;
        RadixSorter sorter;
        int64_t time = runSort(sorter, data);
        bool verified = isSorted(data);
        
        std::cout << std::setw(20) << sorter.getName()
                  << std::setw(20) << formatter.formatDuration(time)
                  << std::setw(15) << (verified ? "OK" : "FAIL") << '\n';
    }

    // BucketSort
    {
        std::vector<uint16_t> data = baseData;
        BucketSorter sorter;
        int64_t time = runSort(sorter, data);
        bool verified = isSorted(data);
        
        std::cout << std::setw(20) << sorter.getName()
                  << std::setw(20) << formatter.formatDuration(time)
                  << std::setw(15) << (verified ? "OK" : "FAIL") << '\n';
    }

    std::cout << std::string(55, '=') << '\n';

    return 0;
}