#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <string>

#include "search/IStringSearch.hpp"
#include "search/AutomatonSearch.hpp"
#include "search/KMPSearchSlow.hpp"
#include "search/KMPSearch.hpp"

/**
 * @brief Генератор случайных строк.
 */
class RandomStringGenerator {
public:
    explicit RandomStringGenerator(unsigned int seed = 42) 
        : gen_(seed) {}
    
    std::string generate(size_t length, const std::string& alphabet = "ABCDEFGHIJ") {
        std::uniform_int_distribution<size_t> dis(0, alphabet.size() - 1);
        
        std::string result;
        result.reserve(length);
        
        for (size_t i = 0; i < length; ++i) {
            result += alphabet[dis(gen_)];
        }
        
        return result;
    }
    
private:
    std::mt19937 gen_;
};

/**
 * @brief Результат бенчмарка.
 */
struct BenchmarkResult {
    std::string algorithmName;
    std::string testName;
    size_t textLength;
    size_t patternLength;
    double avgTimeUs;
    size_t comparisons;
    int position;
};

/**
 * @brief Замер времени построения таблиц/π-функции.
 */
double measurePreprocessing(IStringSearch& searcher, const std::string& pattern, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        searcher.setPattern(pattern);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    return static_cast<double>(duration.count()) / iterations;
}

/**
 * @brief Запуск бенчмарка поиска.
 */
BenchmarkResult runBenchmark(IStringSearch& searcher, 
                              const std::string& text,
                              const std::string& testName,
                              int iterations) {
    BenchmarkResult result;
    result.algorithmName = searcher.getName();
    result.testName = testName;
    result.textLength = text.length();
    result.patternLength = searcher.getPattern().length();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        searcher.resetStats();
        searcher.search(text);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    result.avgTimeUs = static_cast<double>(duration.count()) / iterations;
    
    searcher.resetStats();
    result.position = searcher.search(text);
    result.comparisons = searcher.getComparisonCount();
    
    return result;
}

void printTableHeader() {
    std::cout << "\n";
    std::cout << "|   Алгоритм   |        Тест          |    n     |  m    |  Время (мкс) |  Сравнений   |  Позиция |" << std::endl;
    
    std::cout << "|" << std::string(14, '-')
              << "|" << std::string(22, '-')
              << "|" << std::string(10, '-')
              << "|" << std::string(7, '-')
              << "|" << std::string(14, '-')
              << "|" << std::string(14, '-')
              << "|" << std::string(10, '-')
              << "|" << std::endl;
}

void printResult(const BenchmarkResult& result) {
    std::cout << "| " << std::left << std::setw(12) << result.algorithmName
              << " | " << std::setw(20) << result.testName
              << " | " << std::right << std::setw(8) << result.textLength
              << " | " << std::setw(5) << result.patternLength
              << " | " << std::setw(12) << std::fixed << std::setprecision(2) << result.avgTimeUs
              << " | " << std::setw(12) << result.comparisons
              << " | " << std::setw(8) << result.position
              << " |" << std::endl;
}

void printSeparator() {
    std::cout << "|" << std::string(14, '-')
              << "|" << std::string(22, '-')
              << "|" << std::string(10, '-')
              << "|" << std::string(7, '-')
              << "|" << std::string(14, '-')
              << "|" << std::string(14, '-')
              << "|" << std::string(10, '-')
              << "|" << std::endl;
}

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "  ДЗ-19: Алгоритм Кнута-Морриса-Пратта" << std::endl;
    std::cout << "  Бенчмарки алгоритмов поиска подстроки" << std::endl;
    std::cout << "============================================" << std::endl;
    
    RandomStringGenerator rng(42);
    
    // ==========================================
    // Часть 1: Сравнение времени препроцессинга
    // ==========================================
    
    std::cout << "\n=== Сравнение времени препроцессинга (π-функция) ===" << std::endl;
    std::cout << "| Алгоритм     |      m     |   Время (мкс)   |" << std::endl;
    std::cout << "|" << std::string(14, '-')
              << "|" << std::string(12, '-')
              << "|" << std::string(17, '-')
              << "|" << std::endl;
    
    std::vector<int> patternLengths = {10, 50, 100, 500, 1000};
    
    for (int m : patternLengths) {
        std::string pattern = rng.generate(m);
        
        KMPSearchSlow slow;
        KMPSearch fast;
        AutomatonSearch automaton;
        
        double slowTime = measurePreprocessing(slow, pattern, 100);
        double fastTime = measurePreprocessing(fast, pattern, 1000);
        double autoTime = measurePreprocessing(automaton, pattern, 10);
        
        std::cout << "| " << std::left << std::setw(12) << "KMP-Slow"
                  << " | " << std::setw(10) << m
                  << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(2) << slowTime
                  << " |" << std::endl;
        std::cout << "| " << std::left << std::setw(12) << "KMP"
                  << " | " << std::setw(10) << m
                  << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(2) << fastTime
                  << " |" << std::endl;
        std::cout << "| " << std::left << std::setw(12) << "Automaton"
                  << " | " << std::setw(10) << m
                  << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(2) << autoTime
                  << " |" << std::endl;
        std::cout << "|" << std::string(14, '-')
                  << "|" << std::string(12, '-')
                  << "|" << std::string(17, '-')
                  << "|" << std::endl;
    }
    
    // ==========================================
    // Часть 2: Сравнение времени поиска
    // ==========================================
    
    std::cout << "\n=== Сравнение времени поиска ===" << std::endl;
    
    struct TestCase {
        std::string name;
        std::string text;
        std::string pattern;
        int iterations;
    };
    
    std::vector<TestCase> tests = {
        {"Random short", rng.generate(100000), "ABCDE", 1000},
        {"Random long", rng.generate(100000), rng.generate(50), 1000},
        {"Worst case", std::string(10000, 'A') + "B", std::string(100, 'A') + "B", 100},
        {"Best case", std::string(100000, 'A'), "BCDEFGHIJK", 1000},
        {"ABABC example", "ABABABCCBBABABCAB", "ABABC", 100000},
        {"Many matches", std::string(100, 'A'), "AA", 10000}
    };
    
    printTableHeader();
    
    for (const auto& test : tests) {
        AutomatonSearch automaton;
        KMPSearchSlow kmpSlow;
        KMPSearch kmp;
        
        automaton.setPattern(test.pattern);
        kmpSlow.setPattern(test.pattern);
        kmp.setPattern(test.pattern);
        
        auto autoResult = runBenchmark(automaton, test.text, test.name, test.iterations);
        auto slowResult = runBenchmark(kmpSlow, test.text, test.name, test.iterations);
        auto kmpResult = runBenchmark(kmp, test.text, test.name, test.iterations);
        
        printResult(autoResult);
        printResult(slowResult);
        printResult(kmpResult);
        printSeparator();
    }
    
    // ==========================================
    // Выводы
    // ==========================================
    
    std::cout << "\n";
    std::cout << "============================================" << std::endl;
    std::cout << "  Выводы" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "\n";
    std::cout << "1. Automaton: O(m²·|Σ|) препроцессинг, O(n) поиск.\n";
    std::cout << "   Быстрый поиск, но медленное построение и много памяти.\n";
    std::cout << "\n";
    std::cout << "2. KMP-Slow: O(m³) препроцессинг, O(n) поиск.\n";
    std::cout << "   Наивное вычисление π-функции, только для демонстрации.\n";
    std::cout << "\n";
    std::cout << "3. KMP: O(m) препроцессинг, O(n) поиск.\n";
    std::cout << "   Оптимальная асимптотика O(n+m), мало памяти O(m).\n";
    std::cout << "\n";
    std::cout << "Рекомендации:\n";
    std::cout << "- KMP: универсальный выбор, гарантированная линейность\n";
    std::cout << "- Automaton: многократный поиск одного образца\n";
    std::cout << "- Boyer-Moore: естественные тексты, большой алфавит\n";
    std::cout << "\n";
    
    return 0;
}
