/**
 * @file Benchmark.cpp
 * @brief Сравнительные бенчмарки Trie vs Hash Table
 */

#include "trie/Trie.hpp"
#include "trie/TrieMap.hpp"
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <map>

using namespace std::chrono;

/**
 * @brief Генератор случайных строк
 */
class StringGenerator {
public:
    explicit StringGenerator(unsigned seed = 42) : rng_(seed), charDist_('a', 'z') {}
    
    std::string generate(size_t len) {
        std::string result;
        result.reserve(len);
        for (size_t i = 0; i < len; ++i) {
            result += charDist_(rng_);
        }
        return result;
    }
    
    std::string generate(size_t minLen, size_t maxLen) {
        std::uniform_int_distribution<size_t> lenDist(minLen, maxLen);
        return generate(lenDist(rng_));
    }
    
    std::vector<std::string> generateUnique(size_t count, size_t minLen, size_t maxLen) {
        std::unordered_set<std::string> unique;
        while (unique.size() < count) {
            unique.insert(generate(minLen, maxLen));
        }
        return std::vector<std::string>(unique.begin(), unique.end());
    }
    
    void reset(unsigned seed = 42) {
        rng_.seed(seed);
    }
    
private:
    std::mt19937 rng_;
    std::uniform_int_distribution<char> charDist_;
};

/**
 * @brief Результат бенчмарка
 */
struct BenchmarkResult {
    std::string operation;
    std::string structure;
    size_t dataSize;
    double timeMs;
    double opsPerSec;
};

/**
 * @brief Класс для проведения бенчмарков
 */
class BenchmarkRunner {
public:
    void benchmarkInsert(size_t count) {
        StringGenerator gen(42);
        auto data = gen.generateUnique(count, 5, 15);
        
        // Trie
        {
            Trie trie;
            auto start = high_resolution_clock::now();
            for (const auto& word : data) {
                trie.insert(word);
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"Insert", "Trie", count, ms, count / (ms / 1000.0)});
        }
        
        // HashSet
        {
            std::unordered_set<std::string> hashSet;
            auto start = high_resolution_clock::now();
            for (const auto& word : data) {
                hashSet.insert(word);
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"Insert", "HashSet", count, ms, count / (ms / 1000.0)});
        }
    }
    
    void benchmarkSearch(size_t count) {
        StringGenerator gen(42);
        auto data = gen.generateUnique(count, 5, 15);
        
        // Подготовка
        Trie trie;
        std::unordered_set<std::string> hashSet;
        for (const auto& word : data) {
            trie.insert(word);
            hashSet.insert(word);
        }
        
        std::shuffle(data.begin(), data.end(), std::mt19937(123));
        
        // Trie
        {
            auto start = high_resolution_clock::now();
            for (const auto& word : data) {
                volatile bool found = trie.search(word);
                (void)found;
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"Search", "Trie", count, ms, count / (ms / 1000.0)});
        }
        
        // HashSet
        {
            auto start = high_resolution_clock::now();
            for (const auto& word : data) {
                volatile bool found = hashSet.count(word) > 0;
                (void)found;
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"Search", "HashSet", count, ms, count / (ms / 1000.0)});
        }
    }
    
    void benchmarkRemove(size_t count) {
        StringGenerator gen(42);
        auto data = gen.generateUnique(count, 5, 15);
        
        // Trie
        {
            Trie trie;
            for (const auto& word : data) {
                trie.insert(word);
            }
            std::shuffle(data.begin(), data.end(), std::mt19937(123));
            
            auto start = high_resolution_clock::now();
            for (const auto& word : data) {
                trie.remove(word);
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"Remove", "Trie", count, ms, count / (ms / 1000.0)});
        }
        
        // HashSet
        {
            std::unordered_set<std::string> hashSet(data.begin(), data.end());
            std::shuffle(data.begin(), data.end(), std::mt19937(123));
            
            auto start = high_resolution_clock::now();
            for (const auto& word : data) {
                hashSet.erase(word);
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"Remove", "HashSet", count, ms, count / (ms / 1000.0)});
        }
    }
    
    /**
     * @brief Корректный бенчмарк prefix search
     * 
     * Методика: 
     * - Генерируем N случайных слов (разнообразные префиксы)
     * - Берём реальные префиксы длины 2-3 из вставленных слов
     * - Ищем по этим префиксам (каждый даёт ~небольшое число результатов)
     */
    void benchmarkPrefixSearch(size_t dictSize, size_t numQueries) {
        StringGenerator gen(42);
        auto data = gen.generateUnique(dictSize, 5, 15);
        
        // Подготовка структур
        Trie trie;
        std::unordered_set<std::string> hashSet;
        for (const auto& word : data) {
            trie.insert(word);
            hashSet.insert(word);
        }
        
        // Генерируем запросы — берём префиксы длины 2 из реальных слов
        std::vector<std::string> queries;
        queries.reserve(numQueries);
        std::mt19937 rng(999);
        std::uniform_int_distribution<size_t> dist(0, data.size() - 1);
        for (size_t i = 0; i < numQueries; ++i) {
            const auto& word = data[dist(rng)];
            size_t prefixLen = std::min(size_t(2), word.size());
            queries.push_back(word.substr(0, prefixLen));
        }
        
        size_t totalResultsTrie = 0;
        size_t totalResultsHash = 0;
        
        // Trie prefix search
        {
            auto start = high_resolution_clock::now();
            for (const auto& prefix : queries) {
                auto results = trie.getWordsWithPrefix(prefix);
                totalResultsTrie += results.size();
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"PrefixSearch", "Trie", numQueries, ms, numQueries / (ms / 1000.0)});
        }
        
        // HashSet prefix search (линейный перебор)
        {
            auto start = high_resolution_clock::now();
            for (const auto& prefix : queries) {
                std::vector<std::string> results;
                for (const auto& word : hashSet) {
                    if (word.size() >= prefix.size() && 
                        word.compare(0, prefix.size(), prefix) == 0) {
                        results.push_back(word);
                    }
                }
                totalResultsHash += results.size();
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"PrefixSearch", "HashSet", numQueries, ms, numQueries / (ms / 1000.0)});
        }
        
        std::cout << "  [PrefixSearch] Avg results per query: " 
                  << totalResultsTrie / numQueries << "\n";
    }
    
    /**
     * @brief Бенчмарк autocomplete (реалистичный сценарий)
     * 
     * Симуляция: пользователь вводит символ за символом
     */
    void benchmarkAutocomplete(size_t dictSize) {
        StringGenerator gen(42);
        auto data = gen.generateUnique(dictSize, 5, 15);
        
        Trie trie;
        std::unordered_set<std::string> hashSet;
        for (const auto& word : data) {
            trie.insert(word);
            hashSet.insert(word);
        }
        
        // Симулируем ввод 1000 слов посимвольно
        size_t numWords = 1000;
        std::mt19937 rng(777);
        std::uniform_int_distribution<size_t> dist(0, data.size() - 1);
        
        std::vector<std::string> wordsToType;
        for (size_t i = 0; i < numWords; ++i) {
            wordsToType.push_back(data[dist(rng)]);
        }
        
        size_t totalQueries = 0;
        for (const auto& word : wordsToType) {
            totalQueries += word.size(); // каждый символ = 1 запрос
        }
        
        // Trie autocomplete
        {
            auto start = high_resolution_clock::now();
            for (const auto& word : wordsToType) {
                for (size_t len = 1; len <= word.size(); ++len) {
                    std::string prefix = word.substr(0, len);
                    auto results = trie.getWordsWithPrefix(prefix);
                    (void)results;
                }
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"Autocomplete", "Trie", totalQueries, ms, totalQueries / (ms / 1000.0)});
        }
        
        // HashSet autocomplete
        {
            auto start = high_resolution_clock::now();
            for (const auto& word : wordsToType) {
                for (size_t len = 1; len <= word.size(); ++len) {
                    std::string prefix = word.substr(0, len);
                    std::vector<std::string> results;
                    for (const auto& w : hashSet) {
                        if (w.size() >= prefix.size() && 
                            w.compare(0, prefix.size(), prefix) == 0) {
                            results.push_back(w);
                        }
                    }
                    (void)results;
                }
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"Autocomplete", "HashSet", totalQueries, ms, totalQueries / (ms / 1000.0)});
        }
    }
    
    /**
     * @brief Бенчмарк TrieMap vs unordered_map
     */
    void benchmarkMap(size_t count) {
        StringGenerator gen(42);
        auto keys = gen.generateUnique(count, 5, 15);
        
        // Put
        {
            TrieMap<int> trieMap;
            auto start = high_resolution_clock::now();
            for (size_t i = 0; i < count; ++i) {
                trieMap.put(keys[i], static_cast<int>(i));
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"MapPut", "TrieMap", count, ms, count / (ms / 1000.0)});
        }
        
        {
            std::unordered_map<std::string, int> hashMap;
            auto start = high_resolution_clock::now();
            for (size_t i = 0; i < count; ++i) {
                hashMap[keys[i]] = static_cast<int>(i);
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"MapPut", "HashMap", count, ms, count / (ms / 1000.0)});
        }
        
        // Get
        TrieMap<int> trieMap;
        std::unordered_map<std::string, int> hashMap;
        for (size_t i = 0; i < count; ++i) {
            trieMap.put(keys[i], static_cast<int>(i));
            hashMap[keys[i]] = static_cast<int>(i);
        }
        std::shuffle(keys.begin(), keys.end(), std::mt19937(123));
        
        {
            auto start = high_resolution_clock::now();
            for (const auto& key : keys) {
                volatile auto val = trieMap.get(key);
                (void)val;
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"MapGet", "TrieMap", count, ms, count / (ms / 1000.0)});
        }
        
        {
            auto start = high_resolution_clock::now();
            for (const auto& key : keys) {
                volatile auto it = hashMap.find(key);
                (void)it;
            }
            auto end = high_resolution_clock::now();
            double ms = duration_cast<microseconds>(end - start).count() / 1000.0;
            results_.push_back({"MapGet", "HashMap", count, ms, count / (ms / 1000.0)});
        }
    }
    
    void printResults() const {
        std::cout << "\n";
        std::cout << std::string(80, '=') << "\n";
        std::cout << "               BENCHMARK RESULTS: Trie vs Hash Table\n";
        std::cout << std::string(80, '=') << "\n\n";
        
        std::cout << std::left << std::setw(15) << "Operation"
                  << std::setw(12) << "Structure"
                  << std::setw(12) << "Count"
                  << std::setw(15) << "Time (ms)"
                  << std::setw(15) << "Ops/sec"
                  << "\n";
        std::cout << std::string(69, '-') << "\n";
        
        for (const auto& r : results_) {
            std::cout << std::left << std::setw(15) << r.operation
                      << std::setw(12) << r.structure
                      << std::setw(12) << r.dataSize
                      << std::fixed << std::setprecision(3)
                      << std::setw(15) << r.timeMs
                      << std::scientific << std::setprecision(2)
                      << std::setw(15) << r.opsPerSec
                      << "\n";
        }
        std::cout << "\n";
    }
    
    void printAnalysis() const {
        std::cout << std::string(80, '=') << "\n";
        std::cout << "                          ANALYSIS\n";
        std::cout << std::string(80, '=') << "\n\n";
        
        // Сравниваем по операциям (берём последние результаты каждого типа)
        std::map<std::string, std::pair<double, double>> times; // op -> (trie, hash)
        
        for (const auto& r : results_) {
            bool isTrie = (r.structure == "Trie" || r.structure == "TrieMap");
            if (isTrie) {
                times[r.operation].first = r.timeMs;
            } else {
                times[r.operation].second = r.timeMs;
            }
        }
        
        for (const auto& [op, t] : times) {
            double ratio = t.first / t.second;
            std::cout << std::left << std::setw(15) << op << ": ";
            if (ratio < 1.0) {
                std::cout << "Trie " << std::fixed << std::setprecision(1) 
                          << (1.0 / ratio) << "x FASTER\n";
            } else {
                std::cout << "HashTable " << std::fixed << std::setprecision(1) 
                          << ratio << "x faster\n";
            }
        }
        
        std::cout << "\n";
        std::cout << "CONCLUSIONS:\n";
        std::cout << "- Point operations (Insert/Search/Remove): HashTable wins (O(1) vs O(m))\n";
        std::cout << "- Prefix operations (PrefixSearch/Autocomplete): Trie wins dramatically\n";
        std::cout << "- Use Trie when you need prefix-based queries\n";
        std::cout << "- Use HashTable for simple key-value storage\n\n";
    }
    
    void exportToCSV(const std::string& filename) const {
        std::ofstream file(filename);
        file << "Operation,Structure,Count,TimeMs,OpsPerSec\n";
        for (const auto& r : results_) {
            file << r.operation << "," << r.structure << "," 
                 << r.dataSize << "," << r.timeMs << "," << r.opsPerSec << "\n";
        }
        std::cout << "Results exported to: " << filename << "\n";
    }
    
private:
    std::vector<BenchmarkResult> results_;
};

int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         TRIE vs HASH TABLE BENCHMARK                       ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    BenchmarkRunner runner;
    
    // Базовые операции на 100K элементов
    std::cout << "Running basic operations benchmark (100K elements)...\n";
    runner.benchmarkInsert(100000);
    runner.benchmarkSearch(100000);
    runner.benchmarkRemove(100000);
    runner.benchmarkMap(100000);
    
    // Prefix search — 100K словарь, 10K запросов
    std::cout << "\nRunning prefix search benchmark (100K dict, 10K queries)...\n";
    runner.benchmarkPrefixSearch(100000, 10000);
    
    // Autocomplete — реалистичный сценарий
    std::cout << "\nRunning autocomplete benchmark (100K dict)...\n";
    runner.benchmarkAutocomplete(100000);
    
    runner.printResults();
    runner.printAnalysis();
    runner.exportToCSV("benchmark_results.csv");
    
    return 0;
}