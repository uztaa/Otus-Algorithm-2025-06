#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <memory>
#include <algorithm>

#include "BinarySearchTree.hpp"
#include "AVLTree.hpp"
#include "Treap.hpp"
#include "TimeFormatter.h"

constexpr uint32_t SEED = 12345u;
constexpr int64_t TIMEOUT_NS = 60LL * 1000 * 1000 * 1000; // 1 минута

/**
 * @brief Генерация случайного массива
 */
std::vector<int> generateRandomArray(size_t size, uint32_t seed) {
    std::vector<int> arr;
    arr.reserve(size);
    
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, static_cast<int>(size * 10));
    
    for (size_t i = 0; i < size; ++i) {
        arr.push_back(dist(rng));
    }
    
    return arr;
}

/**
 * @brief Генерация возрастающего массива
 */
std::vector<int> generateSortedArray(size_t size) {
    std::vector<int> arr;
    arr.reserve(size);
    
    for (size_t i = 1; i <= size; ++i) {
        arr.push_back(static_cast<int>(i));
    }
    
    return arr;
}

/**
 * @brief Замер времени операции
 */
template<typename Func>
int64_t measureTime(Func&& func) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

/**
 * @brief Результаты бенчмарка
 */
struct BenchmarkResult {
    std::string treeName;
    std::string dataType;
    int64_t insertTime;
    int64_t searchTime;
    int64_t removeTime;
    int height;
    size_t size;
};

/**
 * @brief Запуск бенчмарка для одного дерева
 */
template<typename TreeType>
BenchmarkResult runBenchmark(const std::vector<int>& data, 
                              const std::vector<int>& searchData,
                              const std::vector<int>& removeData,
                              const std::string& dataType) {
    TreeType tree;
    BenchmarkResult result;
    result.treeName = tree.getName();
    result.dataType = dataType;

    // Insert
    result.insertTime = measureTime([&]() {
        for (int val : data) {
            tree.insert(val);
        }
    });

    result.size = tree.size();
    result.height = tree.height();

    // Search
    result.searchTime = measureTime([&]() {
        for (int val : searchData) {
            tree.search(val);
        }
    });

    // Remove
    result.removeTime = measureTime([&]() {
        for (int val : removeData) {
            tree.remove(val);
        }
    });

    return result;
}

void printResults(const std::vector<BenchmarkResult>& results, size_t N) {
    TimeFormatter formatter;

    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    Benchmark: Binary Trees (N = " << std::setw(7) << N << ")                              ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════════════════╝\n\n";

    std::cout << std::left
              << std::setw(18) << "Tree"
              << std::setw(10) << "Data"
              << std::setw(15) << "Insert"
              << std::setw(15) << "Search"
              << std::setw(15) << "Remove"
              << std::setw(10) << "Height"
              << std::setw(10) << "Size" << '\n';
    std::cout << std::string(93, '=') << '\n';

    for (const auto& r : results) {
        std::cout << std::left
                  << std::setw(18) << r.treeName
                  << std::setw(10) << r.dataType
                  << std::setw(15) << formatter.formatDuration(r.insertTime)
                  << std::setw(15) << formatter.formatDuration(r.searchTime)
                  << std::setw(15) << formatter.formatDuration(r.removeTime)
                  << std::setw(10) << r.height
                  << std::setw(10) << r.size << '\n';
    }

    std::cout << std::string(93, '=') << '\n';
}

int main() {
    // Подбираем N так, чтобы программа работала меньше минуты
    // Для BST с sorted данными критично — вырождение в O(n)
    const size_t N = 50000;  // Можно увеличить для AVL/Treap
    const size_t searchCount = N / 10;
    const size_t removeCount = N / 10;

    std::cout << "Generating test data...\n";

    // Генерация данных
    std::vector<int> randomData = generateRandomArray(N, SEED);
    std::vector<int> sortedData = generateSortedArray(N);

    // Данные для поиска и удаления
    std::mt19937 rng(SEED + 1);
    std::vector<int> searchData;
    std::vector<int> removeData;
    
    std::uniform_int_distribution<int> dist(1, static_cast<int>(N * 10));
    for (size_t i = 0; i < searchCount; ++i) {
        searchData.push_back(dist(rng));
    }
    
    // Для удаления берём элементы из вставленных
    removeData = randomData;
    std::shuffle(removeData.begin(), removeData.end(), rng);
    removeData.resize(removeCount);

    std::vector<BenchmarkResult> results;

    std::cout << "Running benchmarks...\n\n";

    // BST Random
    results.push_back(runBenchmark<BinarySearchTree>(randomData, searchData, removeData, "random"));

    // BST Sorted (может быть очень медленным!)
    std::cout << "BST sorted (may take a while)...\n";
    results.push_back(runBenchmark<BinarySearchTree>(sortedData, searchData, removeData, "sorted"));

    // AVL Random
    results.push_back(runBenchmark<AVLTree>(randomData, searchData, removeData, "random"));

    // AVL Sorted
    results.push_back(runBenchmark<AVLTree>(sortedData, searchData, removeData, "sorted"));

    // Treap Random
    results.push_back(runBenchmark<Treap>(randomData, searchData, removeData, "random"));

    // Treap Sorted
    results.push_back(runBenchmark<Treap>(sortedData, searchData, removeData, "sorted"));

    printResults(results, N);

    std::cout << "\n=== Выводы ===\n";
    std::cout << "1. BST на sorted данных вырождается в список (height ≈ N)\n";
    std::cout << "2. AVL гарантирует O(log N) даже на sorted данных\n";
    std::cout << "3. Treap балансируется вероятностно, height ≈ O(log N)\n";

    return 0;
}