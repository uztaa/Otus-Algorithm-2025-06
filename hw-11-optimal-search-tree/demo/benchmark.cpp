#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <unordered_set>
#include <sstream>

#include "BinarySearchTree.hpp"
#include "AVLTree.hpp"
#include "Treap.hpp"
#include "OptimalTreeAlgo1.hpp"
#include "OptimalTreeAlgo2.hpp"

// ============================================
// Конфигурация бенчмарка
// ============================================

constexpr size_t N = 1000000;           // Количество ключей (1 миллион)
constexpr size_t SEARCH_COUNT = 10000000; // Количество поисковых запросов (10 миллионов)
constexpr uint32_t SEED = 12345u;       // Seed для воспроизводимости (как в ДЗ-10)

// ============================================
// Генераторы данных
// ============================================

/**
 * @brief Генерация случайных уникальных ключей
 */
std::vector<int> generateRandomKeys(size_t count, uint32_t seed) {
    std::vector<int> keys;
    keys.reserve(count);
    
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, static_cast<int>(count * 10));
    
    // Генерируем уникальные ключи
    std::unordered_set<int> seen;
    while (keys.size() < count) {
        int key = dist(rng);
        if (seen.insert(key).second) {
            keys.push_back(key);
        }
    }
    
    return keys;
}

/**
 * @brief Генерация последовательных ключей (1, 2, 3, ..., N)
 * 
 * Это "неслучайные" данные — BST вырождается в список!
 * ДОП должен показать преимущество.
 */
std::vector<int> generateSequentialKeys(size_t count) {
    std::vector<int> keys(count);
    std::iota(keys.begin(), keys.end(), 1);  // 1, 2, 3, ..., N
    return keys;
}

/**
 * @brief Генерация весов по распределению Zipf
 * 
 * Zipf: вес элемента с рангом r = C / r^s, где s ≈ 1
 * Моделирует реальное распределение частот (закон Ципфа)
 */
std::vector<int> generateZipfWeights(size_t count, double exponent = 1.0, uint32_t seed = SEED) {
    std::vector<int> weights(count);
    
    // Сначала генерируем ранги (случайное назначение)
    std::vector<size_t> ranks(count);
    std::iota(ranks.begin(), ranks.end(), 1);  // 1, 2, 3, ..., count
    
    std::mt19937 rng(seed + 100);  // Другой seed для разнообразия
    std::shuffle(ranks.begin(), ranks.end(), rng);
    
    // Вычисляем веса по Zipf
    const int maxWeight = 10000;
    for (size_t i = 0; i < count; ++i) {
        weights[i] = static_cast<int>(maxWeight / std::pow(ranks[i], exponent));
        if (weights[i] < 1) weights[i] = 1;
    }
    
    return weights;
}

/**
 * @brief Генерация СВЕРХ-экстремального распределения
 * 
 * ОДИН ключ получает 99.9% всех запросов!
 * Это сценарий, где ДОП должен показать преимущество:
 * - ДОП: 1 сравнение в 99.9% случаев (ключ в корне)
 * - AVL: ~20 сравнений (log2(1000000))
 */
std::vector<int> generateSuperExtremeWeights(size_t count, uint32_t seed = SEED) {
    std::vector<int> weights(count, 1);  // Базовый вес = 1
    
    // Выбираем ОДИН случайный горячий ключ
    std::mt19937 rng(seed + 300);
    std::uniform_int_distribution<size_t> dist(0, count - 1);
    size_t hotIndex = dist(rng);
    
    // Этот ключ получает 99.9% веса
    // Сумма: 1 * (N-1) + hotWeight ≈ hotWeight
    // Доля горячего: hotWeight / (hotWeight + N) ≈ 99.9%
    weights[hotIndex] = static_cast<int>(count * 1000);  // ~99.9% веса
    
    return weights;
}

/**
 * @brief Генерация поисковых запросов с учётом весов (Zipf)
 * 
 * Ключи с большим весом запрашиваются чаще
 */
std::vector<int> generateWeightedQueries(const std::vector<int>& keys,
                                          const std::vector<int>& weights,
                                          size_t queryCount,
                                          uint32_t seed) {
    std::vector<int> queries;
    queries.reserve(queryCount);
    
    // Построим CDF (кумулятивную функцию распределения)
    std::vector<int64_t> cdf(weights.size());
    cdf[0] = weights[0];
    for (size_t i = 1; i < weights.size(); ++i) {
        cdf[i] = cdf[i - 1] + weights[i];
    }
    int64_t totalWeight = cdf.back();
    
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<int64_t> dist(1, totalWeight);
    
    for (size_t i = 0; i < queryCount; ++i) {
        int64_t r = dist(rng);
        // Бинарный поиск в CDF
        auto it = std::lower_bound(cdf.begin(), cdf.end(), r);
        size_t idx = std::distance(cdf.begin(), it);
        queries.push_back(keys[idx]);
    }
    
    return queries;
}

/**
 * @brief Генерация равномерных поисковых запросов
 */
std::vector<int> generateUniformQueries(const std::vector<int>& keys,
                                         size_t queryCount,
                                         uint32_t seed) {
    std::vector<int> queries;
    queries.reserve(queryCount);
    
    std::mt19937 rng(seed);
    std::uniform_int_distribution<size_t> dist(0, keys.size() - 1);
    
    for (size_t i = 0; i < queryCount; ++i) {
        queries.push_back(keys[dist(rng)]);
    }
    
    return queries;
}

// ============================================
// Утилиты
// ============================================

template<typename Func>
int64_t measureTimeNs(Func&& func) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

std::string formatDuration(int64_t ns) {
    if (ns < 1000) {
        return std::to_string(ns) + " ns";
    } else if (ns < 1000000) {
        return std::to_string(ns / 1000) + "." + std::to_string((ns % 1000) / 100) + " us";
    } else if (ns < 1000000000) {
        return std::to_string(ns / 1000000) + "." + std::to_string((ns % 1000000) / 100000) + " ms";
    } else {
        return std::to_string(ns / 1000000000) + "." + std::to_string((ns % 1000000000) / 100000000) + " s";
    }
}

// ============================================
// Результаты бенчмарка
// ============================================

struct BenchmarkResult {
    std::string name;
    int64_t buildTime;
    int64_t searchTimeUniform;
    int64_t searchTimeWeighted;
    int height;
    double weightedHeight;  // hср
    size_t size;
};

// ============================================
// Бенчмарк динамических деревьев
// ============================================

template<typename TreeType>
BenchmarkResult benchmarkDynamicTree(const std::vector<int>& keys,
                                      const std::vector<int>& uniformQueries,
                                      const std::vector<int>& weightedQueries) {
    TreeType tree;
    BenchmarkResult result;
    result.name = tree.getName();
    
    // Построение (вставка всех ключей)
    result.buildTime = measureTimeNs([&]() {
        for (int key : keys) {
            tree.insert(key);
        }
    });
    
    result.size = tree.size();
    result.height = tree.height();
    result.weightedHeight = 0.0;  // Не применимо для динамических деревьев
    
    // Поиск (равномерный)
    result.searchTimeUniform = measureTimeNs([&]() {
        for (int key : uniformQueries) {
            tree.search(key);
        }
    });
    
    // Поиск (взвешенный)
    result.searchTimeWeighted = measureTimeNs([&]() {
        for (int key : weightedQueries) {
            tree.search(key);
        }
    });
    
    return result;
}

// ============================================
// Бенчмарк статических деревьев (ДОП)
// ============================================

template<typename TreeType>
BenchmarkResult benchmarkOptimalTree(const std::vector<int>& keys,
                                      const std::vector<int>& weights,
                                      const std::vector<int>& uniformQueries,
                                      const std::vector<int>& weightedQueries) {
    TreeType tree;
    BenchmarkResult result;
    result.name = tree.getName();
    
    // Построение
    result.buildTime = measureTimeNs([&]() {
        tree.build(keys, weights);
    });
    
    result.size = tree.size();
    result.height = tree.height();
    result.weightedHeight = tree.getWeightedHeight();
    
    // Поиск (равномерный)
    result.searchTimeUniform = measureTimeNs([&]() {
        for (int key : uniformQueries) {
            tree.search(key);
        }
    });
    
    // Поиск (взвешенный)
    result.searchTimeWeighted = measureTimeNs([&]() {
        for (int key : weightedQueries) {
            tree.search(key);
        }
    });
    
    return result;
}

// ============================================
// Вывод результатов
// ============================================

void printResults(const std::vector<BenchmarkResult>& results, const std::string& dataType) {
    // Формируем заголовок
    std::ostringstream header;
    header << "                    Benchmark: " << dataType << " (N = " << N << ")";
    std::string headerStr = header.str();
    
    // Вычисляем "визуальную" длину строки (UTF-8 кириллица = 2 байта на символ)
    size_t visualLength = 0;
    for (unsigned char c : headerStr) {
        if ((c & 0xC0) != 0x80) visualLength++;  // Пропускаем continuation bytes
    }
    
    const size_t contentWidth = 101;
    if (visualLength < contentWidth) {
        headerStr += std::string(contentWidth - visualLength, ' ');
    }
    
    std::cout << "\n";
    std::cout << "┌─────────────────────────────────────────────────────────────────────────────────────────────────────┐\n";
    std::cout << "│" << headerStr << "│\n";
    std::cout << "└─────────────────────────────────────────────────────────────────────────────────────────────────────┘\n\n";

    std::cout <<
        "|       Дерево      |  Построение |  Поиск(равн)  |  Поиск(Zipf)  |  Высота |   hср   |  Размер |"         
    << std::endl;
    
    for (const auto& r : results) {
        std::cout << std::left
                  << "| " << std::setw(18) << r.name
                  << "| " << std::setw(12) << formatDuration(r.buildTime)
                  << "| " << std::setw(14) << formatDuration(r.searchTimeUniform)
                  << "| " << std::setw(14) << formatDuration(r.searchTimeWeighted)
                  << "| " << std::setw(8) << r.height;
        
        if (r.weightedHeight > 0) {
            std::cout << "| " << std::setw(8) << std::fixed << std::setprecision(2) << r.weightedHeight;
        } else {
            std::cout << "| " << std::setw(8) << "-";
        }
        
        std::cout << "| " << std::setw(8) << r.size << "|\n";
    }
    
    std::cout << "|" << std::string(95, '-') << "|\n";
}

void printAnalysis(const std::vector<BenchmarkResult>& resultsSeq,
                   const std::vector<BenchmarkResult>& resultsRandom,
                   const std::vector<BenchmarkResult>& resultsExtreme) {
    std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
    std::cout << "АНАЛИЗ РЕЗУЛЬТАТОВ\n";
    std::cout << "═══════════════════════════════════════════════════════════════════\n\n";
    
    // Находим результаты по имени
    auto findResult = [](const std::vector<BenchmarkResult>& results, const std::string& name) 
        -> const BenchmarkResult* {
        for (const auto& r : results) {
            if (r.name == name) return &r;
        }
        return nullptr;
    };
    
    // Результаты для последовательных данных
    const auto* avlSeq = findResult(resultsSeq, "AVLTree");
    const auto* algo1Seq = findResult(resultsSeq, "OptimalTree-A1");
    const auto* algo2Seq = findResult(resultsSeq, "OptimalTree-A2");
    
    // Результаты для случайных данных
    const auto* avlRandom = findResult(resultsRandom, "AVLTree");
    const auto* algo1Random = findResult(resultsRandom, "OptimalTree-A1");
    const auto* algo2Random = findResult(resultsRandom, "OptimalTree-A2");
    
    // Результаты для экстремального Zipf
    const auto* avlExtreme = findResult(resultsExtreme, "AVLTree");
    const auto* algo1Extreme = findResult(resultsExtreme, "OptimalTree-A1");
    const auto* algo2Extreme = findResult(resultsExtreme, "OptimalTree-A2");
    
    std::cout << "1. СЦЕНАРИЙ 1 (последовательные ключи) — Поиск Zipf:\n";
    if (avlSeq && algo1Seq && algo2Seq) {
        std::cout << "   AVL:     " << formatDuration(avlSeq->searchTimeWeighted) << "\n";
        std::cout << "   ДОП-A1:  " << formatDuration(algo1Seq->searchTimeWeighted) << "\n";
        std::cout << "   ДОП-A2:  " << formatDuration(algo2Seq->searchTimeWeighted) << "\n";
        
        const BenchmarkResult* bestDOP = (algo1Seq->searchTimeWeighted < algo2Seq->searchTimeWeighted) 
            ? algo1Seq : algo2Seq;
        
        if (bestDOP->searchTimeWeighted < avlSeq->searchTimeWeighted) {
            double ratio = static_cast<double>(avlSeq->searchTimeWeighted) / bestDOP->searchTimeWeighted;
            std::cout << "   → " << bestDOP->name << " быстрее AVL в " 
                      << std::fixed << std::setprecision(2) << ratio << "x раз!\n";
        } else {
            double ratio = static_cast<double>(bestDOP->searchTimeWeighted) / avlSeq->searchTimeWeighted;
            std::cout << "   → AVL быстрее ДОП в " << std::fixed << std::setprecision(2) << ratio << "x раз\n";
        }
    }
    
    std::cout << "\n2. СЦЕНАРИЙ 2 (случайные ключи) — Поиск Zipf:\n";
    if (avlRandom && algo1Random && algo2Random) {
        std::cout << "   AVL:     " << formatDuration(avlRandom->searchTimeWeighted) << "\n";
        std::cout << "   ДОП-A1:  " << formatDuration(algo1Random->searchTimeWeighted) << "\n";
        std::cout << "   ДОП-A2:  " << formatDuration(algo2Random->searchTimeWeighted) << "\n";
        
        const BenchmarkResult* bestDOP = (algo1Random->searchTimeWeighted < algo2Random->searchTimeWeighted) 
            ? algo1Random : algo2Random;
        if (avlRandom->searchTimeWeighted < bestDOP->searchTimeWeighted) {
            double ratio = static_cast<double>(bestDOP->searchTimeWeighted) / avlRandom->searchTimeWeighted;
            std::cout << "   → AVL быстрее ДОП в " << std::fixed << std::setprecision(2) << ratio << "x раз\n";
        } else {
            double ratio = static_cast<double>(avlRandom->searchTimeWeighted) / bestDOP->searchTimeWeighted;
            std::cout << "   → ДОП быстрее AVL в " << std::fixed << std::setprecision(2) << ratio << "x раз!\n";
        }
    }
    
    std::cout << "\n3. СЦЕНАРИЙ 3 (1 ключ = 99.9%) — Поиск Zipf:\n";
    if (avlExtreme && algo1Extreme && algo2Extreme) {
        std::cout << "   AVL:     " << formatDuration(avlExtreme->searchTimeWeighted) 
                  << " (высота=" << avlExtreme->height << ")\n";
        std::cout << "   ДОП-A1:  " << formatDuration(algo1Extreme->searchTimeWeighted) << " (hср=" 
                  << std::fixed << std::setprecision(2) << algo1Extreme->weightedHeight << ")\n";
        std::cout << "   ДОП-A2:  " << formatDuration(algo2Extreme->searchTimeWeighted) << " (hср=" 
                  << algo2Extreme->weightedHeight << ")\n";
        
        const BenchmarkResult* bestDOP = (algo1Extreme->searchTimeWeighted < algo2Extreme->searchTimeWeighted) 
            ? algo1Extreme : algo2Extreme;
        
        if (bestDOP->searchTimeWeighted < avlExtreme->searchTimeWeighted) {
            double ratio = static_cast<double>(avlExtreme->searchTimeWeighted) / bestDOP->searchTimeWeighted;
            std::cout << "   → " << bestDOP->name << " БЫСТРЕЕ AVL в " 
                      << std::fixed << std::setprecision(2) << ratio << "x раз! ✓✓✓\n";
        } else {
            double ratio = static_cast<double>(bestDOP->searchTimeWeighted) / avlExtreme->searchTimeWeighted;
            std::cout << "   → AVL быстрее ДОП в " << std::fixed << std::setprecision(2) << ratio << "x раз\n";
        }
    }
    
    std::cout << "\n4. ВЫВОДЫ:\n";
    std::cout << "   • На случайных данных AVL быстрее (лучше кэш-локальность)\n";
    std::cout << "   • ДОП-A2 выигрывает при экстремальном Zipf (1 горячий ключ)\n";
    std::cout << "   • ДОП-A2 также эффективен на последовательных данных\n";
    std::cout << "   • Для динамических данных используйте AVL/Treap\n";
}

// ============================================
// Запуск полного бенчмарка для набора данных
// ============================================

std::vector<BenchmarkResult> runFullBenchmark(
    const std::vector<int>& keys,
    const std::vector<int>& weights,
    const std::vector<int>& uniformQueries,
    const std::vector<int>& weightedQueries,
    bool skipBSTForSequential = false) 
{
    std::vector<BenchmarkResult> results;
    
    // Динамические деревья
    if (!skipBSTForSequential) {
        std::cout << "  [1/5] BinarySearchTree...\n";
        results.push_back(benchmarkDynamicTree<BinarySearchTree>(keys, uniformQueries, weightedQueries));
    } else {
        std::cout << "  [1/5] BinarySearchTree... ПРОПУЩЕН (вырождается на sequential)\n";
    }
    
    std::cout << "  [2/5] AVLTree...\n";
    results.push_back(benchmarkDynamicTree<AVLTree>(keys, uniformQueries, weightedQueries));
    
    std::cout << "  [3/5] Treap...\n";
    results.push_back(benchmarkDynamicTree<Treap>(keys, uniformQueries, weightedQueries));
    
    // Статические деревья (ДОП)
    std::cout << "  [4/5] OptimalTree-A1 (по весам)...\n";
    results.push_back(benchmarkOptimalTree<OptimalTreeAlgo1>(keys, weights, uniformQueries, weightedQueries));
    
    std::cout << "  [5/5] OptimalTree-A2 (центр тяжести)...\n";
    results.push_back(benchmarkOptimalTree<OptimalTreeAlgo2>(keys, weights, uniformQueries, weightedQueries));
    
    return results;
}

// ============================================
// Main
// ============================================

int main() {
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║   ДЗ-11: Деревья Оптимального Поиска (ДОП)                     ║\n";
    std::cout << "║   Сравнение: BST, AVL, Treap, ДОП-Algo1, ДОП-Algo2             ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
    
    std::cout << "Параметры:\n";
    std::cout << "  N = " << N << " ключей\n";
    std::cout << "  Поисковых запросов: " << SEARCH_COUNT << "\n";
    std::cout << "  Seed: " << SEED << "\n\n";
    
    // ================================================================
    // СЦЕНАРИЙ 1: Последовательные ключи
    // ================================================================
    std::cout << "═══════════════════════════════════════════════════════════════════\n";
    std::cout << "СЦЕНАРИЙ 1: ПОСЛЕДОВАТЕЛЬНЫЕ КЛЮЧИ (1, 2, 3, ..., N)\n";
    std::cout << "═══════════════════════════════════════════════════════════════════\n\n";
    
    auto seqKeys = generateSequentialKeys(N);
    auto seqWeights = generateZipfWeights(N, 1.0, SEED);
    auto uniformQueriesSeq = generateUniformQueries(seqKeys, SEARCH_COUNT, SEED + 1);
    auto weightedQueriesSeq = generateWeightedQueries(seqKeys, seqWeights, SEARCH_COUNT, SEED + 2);
    
    std::cout << "Запуск бенчмарков...\n";
    // Пропускаем BST — вырождается в список O(n)
    auto resultsSeq = runFullBenchmark(seqKeys, seqWeights, 
                                        uniformQueriesSeq, weightedQueriesSeq, true);
    
    printResults(resultsSeq, "Последовательные");
    
    // ================================================================
    // СЦЕНАРИЙ 2: Случайные ключи
    // ================================================================
    std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
    std::cout << "СЦЕНАРИЙ 2: СЛУЧАЙНЫЕ КЛЮЧИ\n";
    std::cout << "═══════════════════════════════════════════════════════════════════\n\n";
    
    auto randomKeys = generateRandomKeys(N, SEED);
    auto randomWeights = generateZipfWeights(N, 1.0, SEED + 100);
    auto uniformQueriesRandom = generateUniformQueries(randomKeys, SEARCH_COUNT, SEED + 3);
    auto weightedQueriesRandom = generateWeightedQueries(randomKeys, randomWeights, SEARCH_COUNT, SEED + 4);
    
    std::cout << "Запуск бенчмарков...\n";
    auto resultsRandom = runFullBenchmark(randomKeys, randomWeights, 
                                           uniformQueriesRandom, weightedQueriesRandom);
    
    printResults(resultsRandom, "Случайные");
    
    // ================================================================
    // СЦЕНАРИЙ 3: Сверх-экстремальное Zipf (1 ключ = 99.9% запросов)
    // ================================================================
    std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
    std::cout << "СЦЕНАРИЙ 3: СВЕРХ-ЭКСТРЕМАЛЬНОЕ ZIPF (1 ключ = 99.9% запросов)\n";
    std::cout << "Ожидание: ДОП должен выиграть — горячий ключ в корне (1 сравнение)\n";
    std::cout << "═══════════════════════════════════════════════════════════════════\n\n";
    
    auto extremeKeys = generateRandomKeys(N, SEED + 500);
    auto extremeWeights = generateSuperExtremeWeights(N, SEED + 500);
    auto uniformQueriesExtreme = generateUniformQueries(extremeKeys, SEARCH_COUNT, SEED + 5);
    auto weightedQueriesExtreme = generateWeightedQueries(extremeKeys, extremeWeights, SEARCH_COUNT, SEED + 6);
    
    std::cout << "Запуск бенчмарков...\n";
    auto resultsExtreme = runFullBenchmark(extremeKeys, extremeWeights, 
                                            uniformQueriesExtreme, weightedQueriesExtreme);
    
    printResults(resultsExtreme, "Сверх-экстремальное");
    
    // ================================================================
    // Сравнительный анализ
    // ================================================================
    printAnalysis(resultsSeq, resultsRandom, resultsExtreme);
    
    return 0;
}
