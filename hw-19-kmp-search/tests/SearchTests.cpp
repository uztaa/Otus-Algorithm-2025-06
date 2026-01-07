#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

#include "search/IStringSearch.hpp"
#include "search/AutomatonSearch.hpp"
#include "search/KMPSearchSlow.hpp"
#include "search/KMPSearch.hpp"

// ============================================
// Параметризованные тесты для всех алгоритмов
// ============================================

enum class AlgorithmType {
    Automaton,
    KMPSlow,
    KMP
};

std::string algorithmName(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::Automaton: return "Automaton";
        case AlgorithmType::KMPSlow: return "KMPSlow";
        case AlgorithmType::KMP: return "KMP";
    }
    return "Unknown";
}

class SearchTest : public ::testing::TestWithParam<AlgorithmType> {
protected:
    std::unique_ptr<IStringSearch> createSearcher() {
        switch (GetParam()) {
            case AlgorithmType::Automaton:
                return std::make_unique<AutomatonSearch>();
            case AlgorithmType::KMPSlow:
                return std::make_unique<KMPSearchSlow>();
            case AlgorithmType::KMP:
                return std::make_unique<KMPSearch>();
        }
        return nullptr;
    }
};

// ============================================
// Базовые тесты search()
// ============================================

TEST_P(SearchTest, EmptyPattern) {
    auto searcher = createSearcher();
    searcher->setPattern("");
    EXPECT_EQ(searcher->search("ABC"), -1);
    EXPECT_EQ(searcher->search(""), -1);
}

TEST_P(SearchTest, EmptyText) {
    auto searcher = createSearcher();
    searcher->setPattern("ABC");
    EXPECT_EQ(searcher->search(""), -1);
}

TEST_P(SearchTest, PatternLongerThanText) {
    auto searcher = createSearcher();
    searcher->setPattern("ABCDEF");
    EXPECT_EQ(searcher->search("ABC"), -1);
}

TEST_P(SearchTest, PatternEqualsText) {
    auto searcher = createSearcher();
    searcher->setPattern("ABC");
    EXPECT_EQ(searcher->search("ABC"), 0);
}

TEST_P(SearchTest, PatternAtBeginning) {
    auto searcher = createSearcher();
    searcher->setPattern("ABC");
    EXPECT_EQ(searcher->search("ABCDEF"), 0);
}

TEST_P(SearchTest, PatternAtEnd) {
    auto searcher = createSearcher();
    searcher->setPattern("DEF");
    EXPECT_EQ(searcher->search("ABCDEF"), 3);
}

TEST_P(SearchTest, PatternInMiddle) {
    auto searcher = createSearcher();
    searcher->setPattern("CD");
    EXPECT_EQ(searcher->search("ABCDEF"), 2);
}

TEST_P(SearchTest, PatternNotFound) {
    auto searcher = createSearcher();
    searcher->setPattern("XYZ");
    EXPECT_EQ(searcher->search("ABCDEF"), -1);
}

TEST_P(SearchTest, SingleCharPattern) {
    auto searcher = createSearcher();
    searcher->setPattern("A");
    EXPECT_EQ(searcher->search("BANANA"), 1);
}

TEST_P(SearchTest, SingleCharText) {
    auto searcher = createSearcher();
    searcher->setPattern("A");
    EXPECT_EQ(searcher->search("A"), 0);
    
    searcher->setPattern("B");
    EXPECT_EQ(searcher->search("A"), -1);
}

TEST_P(SearchTest, RepeatingPattern) {
    auto searcher = createSearcher();
    searcher->setPattern("AAA");
    EXPECT_EQ(searcher->search("AAAAAAA"), 0);
}

// ============================================
// Тесты searchAll()
// ============================================

TEST_P(SearchTest, SearchAllMultipleOccurrences) {
    auto searcher = createSearcher();
    searcher->setPattern("AB");
    auto results = searcher->searchAll("ABABAB");
    
    ASSERT_EQ(results.size(), 3u);
    EXPECT_EQ(results[0], 0);
    EXPECT_EQ(results[1], 2);
    EXPECT_EQ(results[2], 4);
}

TEST_P(SearchTest, SearchAllOverlapping) {
    auto searcher = createSearcher();
    searcher->setPattern("ABA");
    auto results = searcher->searchAll("ABABA");
    
    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[0], 0);
    EXPECT_EQ(results[1], 2);
}

TEST_P(SearchTest, SearchAllNoOccurrences) {
    auto searcher = createSearcher();
    searcher->setPattern("XYZ");
    auto results = searcher->searchAll("ABCDEF");
    
    EXPECT_TRUE(results.empty());
}

TEST_P(SearchTest, SearchAllSingleOccurrence) {
    auto searcher = createSearcher();
    searcher->setPattern("CD");
    auto results = searcher->searchAll("ABCDEF");
    
    ASSERT_EQ(results.size(), 1u);
    EXPECT_EQ(results[0], 2);
}

TEST_P(SearchTest, SearchAllEmptyPattern) {
    auto searcher = createSearcher();
    searcher->setPattern("");
    auto results = searcher->searchAll("ABC");
    
    EXPECT_TRUE(results.empty());
}

// ============================================
// Тесты из презентации
// ============================================

TEST_P(SearchTest, ABCExample) {
    auto searcher = createSearcher();
    searcher->setPattern("ABABC");
    // Текст из презентации: ABABABCCBBABABCAB
    auto results = searcher->searchAll("ABABABCCBBABABCAB");
    
    // Позиции: 2 и 10
    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[0], 2);
    EXPECT_EQ(results[1], 10);
}

// ============================================
// Тесты переиспользования объекта
// ============================================

TEST_P(SearchTest, ReuseWithSetPattern) {
    auto searcher = createSearcher();
    
    searcher->setPattern("ABC");
    EXPECT_EQ(searcher->search("ABCDEF"), 0);
    EXPECT_EQ(searcher->getPattern(), "ABC");
    
    searcher->setPattern("DEF");
    EXPECT_EQ(searcher->search("ABCDEF"), 3);
    EXPECT_EQ(searcher->getPattern(), "DEF");
}

TEST_P(SearchTest, StatsResetOnSetPattern) {
    auto searcher = createSearcher();
    
    searcher->setPattern("ABC");
    searcher->search("ABCABCABC");
    size_t firstCount = searcher->getComparisonCount();
    EXPECT_GT(firstCount, 0u);
    
    searcher->setPattern("XYZ");
    EXPECT_EQ(searcher->getComparisonCount(), 0u);
}

TEST_P(SearchTest, ManualStatsReset) {
    auto searcher = createSearcher();
    
    searcher->setPattern("ABC");
    searcher->search("ABCABCABC");
    EXPECT_GT(searcher->getComparisonCount(), 0u);
    
    searcher->resetStats();
    EXPECT_EQ(searcher->getComparisonCount(), 0u);
}

// ============================================
// Специальные случаи
// ============================================

TEST_P(SearchTest, WorstCaseRepeating) {
    auto searcher = createSearcher();
    
    std::string text(100, 'A');
    searcher->setPattern("AAAB");
    EXPECT_EQ(searcher->search(text), -1);
}

TEST_P(SearchTest, LongPatternInLongText) {
    auto searcher = createSearcher();
    
    std::string text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    text += text;
    
    searcher->setPattern("MNOPQRST");
    
    int pos = searcher->search(text);
    EXPECT_EQ(pos, 12);
    
    auto all = searcher->searchAll(text);
    ASSERT_EQ(all.size(), 2u);
    EXPECT_EQ(all[0], 12);
    EXPECT_EQ(all[1], 38);
}

TEST_P(SearchTest, GetNameNotEmpty) {
    auto searcher = createSearcher();
    EXPECT_FALSE(searcher->getName().empty());
}

// ============================================
// Параметризация
// ============================================

INSTANTIATE_TEST_SUITE_P(
    AllAlgorithms,
    SearchTest,
    ::testing::Values(
        AlgorithmType::Automaton,
        AlgorithmType::KMPSlow,
        AlgorithmType::KMP
    ),
    [](const testing::TestParamInfo<AlgorithmType>& info) {
        return algorithmName(info.param);
    }
);

// ============================================
// Отдельные тесты для конструкторов
// ============================================

TEST(AutomatonSearchTest, ConstructorWithPattern) {
    AutomatonSearch searcher("ABC");
    EXPECT_EQ(searcher.getPattern(), "ABC");
    EXPECT_EQ(searcher.search("ABCDEF"), 0);
}

TEST(KMPSearchSlowTest, ConstructorWithPattern) {
    KMPSearchSlow searcher("ABC");
    EXPECT_EQ(searcher.getPattern(), "ABC");
    EXPECT_EQ(searcher.search("ABCDEF"), 0);
}

TEST(KMPSearchTest, ConstructorWithPattern) {
    KMPSearch searcher("ABC");
    EXPECT_EQ(searcher.getPattern(), "ABC");
    EXPECT_EQ(searcher.search("ABCDEF"), 0);
}

// ============================================
// Тесты π-функции
// ============================================

TEST(PrefixFunctionTest, SlowAndFastProduceSameResult) {
    std::vector<std::string> patterns = {
        "ABABC",
        "AAAA",
        "ABCABC",
        "ABACABA",
        "A",
        "AB",
        "",
        "AABAAAB"
    };
    
    for (const auto& pattern : patterns) {
        KMPSearchSlow slow(pattern);
        KMPSearch fast(pattern);
        
        EXPECT_EQ(slow.getPrefixFunction(), fast.getPrefixFunction())
            << "Mismatch for pattern: " << pattern;
    }
}

TEST(PrefixFunctionTest, ABABC) {
    KMPSearch kmp("ABABC");
    const auto& pi = kmp.getPrefixFunction();
    
    // A B A B C
    // 0 0 1 2 0
    ASSERT_EQ(pi.size(), 5u);
    EXPECT_EQ(pi[0], 0);
    EXPECT_EQ(pi[1], 0);
    EXPECT_EQ(pi[2], 1);
    EXPECT_EQ(pi[3], 2);
    EXPECT_EQ(pi[4], 0);
}

TEST(PrefixFunctionTest, AAAA) {
    KMPSearch kmp("AAAA");
    const auto& pi = kmp.getPrefixFunction();
    
    // A A A A
    // 0 1 2 3
    ASSERT_EQ(pi.size(), 4u);
    EXPECT_EQ(pi[0], 0);
    EXPECT_EQ(pi[1], 1);
    EXPECT_EQ(pi[2], 2);
    EXPECT_EQ(pi[3], 3);
}

TEST(PrefixFunctionTest, ABCABC) {
    KMPSearch kmp("ABCABC");
    const auto& pi = kmp.getPrefixFunction();
    
    // A B C A B C
    // 0 0 0 1 2 3
    ASSERT_EQ(pi.size(), 6u);
    EXPECT_EQ(pi[0], 0);
    EXPECT_EQ(pi[1], 0);
    EXPECT_EQ(pi[2], 0);
    EXPECT_EQ(pi[3], 1);
    EXPECT_EQ(pi[4], 2);
    EXPECT_EQ(pi[5], 3);
}

TEST(PrefixFunctionTest, ABACABA) {
    KMPSearch kmp("ABACABA");
    const auto& pi = kmp.getPrefixFunction();
    
    // A B A C A B A
    // 0 0 1 0 1 2 3
    ASSERT_EQ(pi.size(), 7u);
    EXPECT_EQ(pi[0], 0);
    EXPECT_EQ(pi[1], 0);
    EXPECT_EQ(pi[2], 1);
    EXPECT_EQ(pi[3], 0);
    EXPECT_EQ(pi[4], 1);
    EXPECT_EQ(pi[5], 2);
    EXPECT_EQ(pi[6], 3);
}

// ============================================
// Тест: KMP находит то же, что и медленный
// ============================================

TEST(KMPConsistencyTest, SlowAndFastFindSamePositions) {
    std::vector<std::pair<std::string, std::string>> testCases = {
        {"ABABC", "ABABABCCBBABABCAB"},
        {"AAA", "AAAAAAA"},
        {"ABC", "ABCABCABC"},
        {"XY", "XXXXYXYXYYYY"},
        {"ABAB", "ABABABABAB"}
    };
    
    for (const auto& [pattern, text] : testCases) {
        KMPSearchSlow slow(pattern);
        KMPSearch fast(pattern);
        
        EXPECT_EQ(slow.search(text), fast.search(text))
            << "search() mismatch for pattern=" << pattern << ", text=" << text;
        
        EXPECT_EQ(slow.searchAll(text), fast.searchAll(text))
            << "searchAll() mismatch for pattern=" << pattern << ", text=" << text;
    }
}
