#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <string>

#include "search/IStringSearch.hpp"
#include "search/NaiveSearch.hpp"
#include "search/BMHSearch.hpp"
#include "search/BoyerMooreSearch.hpp"

#include <Timer.h>

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
    double avgTimeUs;       // Микросекунды
    size_t comparisons;
    int position;
};

/**
 * @brief Запуск бенчмарка для одного алгоритма.
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
    
    // Замер времени
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        searcher.resetStats();
        searcher.search(text);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    result.avgTimeUs = static_cast<double>(duration.count()) / iterations;
    
    // Получаем результат последнего запуска для статистики
    searcher.resetStats();
    result.position = searcher.search(text);
    result.comparisons = searcher.getComparisonCount();
    
    return result;
}

/**
 * @brief Вывод заголовка таблицы.
 */
void printTableHeader() {
    std::cout << "\n";

std::cout << "| Алгоритм     |       Тест           |    n     |  m    |  Время (мкс) |  Сравнений   | Позиция  |" << std::endl;
    
    std::cout << "|" << std::string(14, '-')
              << "|" << std::string(22, '-')
              << "|" << std::string(10, '-')
              << "|" << std::string(7, '-')
              << "|" << std::string(14, '-')
              << "|" << std::string(14, '-')
              << "|" << std::string(10, '-')
              << "|" << std::endl;
}

/**
 * @brief Вывод результата бенчмарка.
 */
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

/**
 * @brief Разделитель между группами тестов.
 */
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
    std::cout << "  ДЗ-18: Алгоритм Бойера-Мура" << std::endl;
    std::cout << "  Бенчмарки алгоритмов поиска подстроки" << std::endl;
    std::cout << "============================================" << std::endl;
    
    RandomStringGenerator rng(42);
    
    // Тестовые сценарии
    struct TestCase {
        std::string name;
        std::string text;
        std::string pattern;
        int iterations;
    };
    
    std::vector<TestCase> tests = {
        // 1. Случайный текст, короткий образец
        {
            "Random short",
            rng.generate(100000),
            "ABCDE",
            1000
        },
        
        // 2. Случайный текст, длинный образец
        {
            "Random long",
            rng.generate(100000),
            rng.generate(50),
            1000
        },
        
        // 3. Худший случай для наивного алгоритма
        {
            "Worst naive",
            std::string(10000, 'A') + "B",
            std::string(100, 'A') + "B",
            100
        },
        
        // 4. Лучший случай для Boyer-Moore
        {
            "Best BM",
            std::string(100000, 'A'),
            "BCDEFGHIJK",
            1000
        },
        
        // 5. Пример из презентации
        {
            "Kolokol",
            "KOLKOKOLOKOLL",
            "KOLOKOL",
            100000
        },
        
        // 6. Реальный текст (повторяющийся)
        {
            "Lorem ipsum",
            std::string(1000, ' ') + "Lorem" + std::string(1000, ' '),
            "Lorem",
            10000
        },
        
        // 7. Много вхождений
        {
            "Many matches",
            std::string(100, 'A'),
            "AA",
            10000
        }
    };
    
    printTableHeader();
    
    for (const auto& test : tests) {
        // Создаём поисковики
        NaiveSearch naive;
        BMHSearch bmh;
        BoyerMooreSearch bm;
        
        naive.setPattern(test.pattern);
        bmh.setPattern(test.pattern);
        bm.setPattern(test.pattern);
        
        // Запускаем бенчмарки
        auto naiveResult = runBenchmark(naive, test.text, test.name, test.iterations);
        auto bmhResult = runBenchmark(bmh, test.text, test.name, test.iterations);
        auto bmResult = runBenchmark(bm, test.text, test.name, test.iterations);
        
        printResult(naiveResult);
        printResult(bmhResult);
        printResult(bmResult);
        printSeparator();
    }
    
    // Выводы
    std::cout << "\n";
    std::cout << "============================================" << std::endl;
    std::cout << "  Выводы" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "\n";
    std::cout << "1. Naive: простейший алгоритм, O(n*m) в худшем случае.\n";
    std::cout << "   Сравнивает слева направо, сдвигается на 1 при несовпадении.\n";
    std::cout << "\n";
    std::cout << "2. BMH (Boyer-Moore-Horspool): использует Bad Character Rule.\n";
    std::cout << "   Сравнивает справа налево, пропускает много позиций.\n";
    std::cout << "   Лучший случай O(n/m), худший O(n*m).\n";
    std::cout << "\n";
    std::cout << "3. Boyer-Moore: Bad Character + Good Suffix Rules.\n";
    std::cout << "   Оба правила — выбирается максимальный сдвиг.\n";
    std::cout << "   Лучший случай O(n/m), худший O(n+m).\n";
    std::cout << "\n";
    std::cout << "Рекомендации:\n";
    std::cout << "- Для коротких образцов: BMH (проще, быстрее препроцессинг)\n";
    std::cout << "- Для длинных образцов: Boyer-Moore (лучше использует суффиксы)\n";
    std::cout << "- Для гарантированной линейности: KMP (не реализован здесь)\n";
    std::cout << "\n";
    
    return 0;
}
