#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

#include "search/IStringSearch.hpp"
#include "search/NaiveSearch.hpp"
#include "search/BMHSearch.hpp"
#include "search/BoyerMooreSearch.hpp"

// ============================================
// Параметризованные тесты для всех алгоритмов
// ============================================

enum class AlgorithmType {
    Naive,
    BMH,
    BoyerMoore
};

std::string algorithmName(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::Naive: return "Naive";
        case AlgorithmType::BMH: return "BMH";
        case AlgorithmType::BoyerMoore: return "BoyerMoore";
    }
    return "Unknown";
}

class SearchTest : public ::testing::TestWithParam<AlgorithmType> {
protected:
    std::unique_ptr<IStringSearch> createSearcher() {
        switch (GetParam()) {
            case AlgorithmType::Naive:
                return std::make_unique<NaiveSearch>();
            case AlgorithmType::BMH:
                return std::make_unique<BMHSearch>();
            case AlgorithmType::BoyerMoore:
                return std::make_unique<BoyerMooreSearch>();
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
    EXPECT_EQ(searcher->search("ABC"), 0);
    EXPECT_EQ(searcher->search(""), 0);
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
    EXPECT_EQ(searcher->search("BANANA"), 1);  // Первое 'A' на позиции 1
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

TEST_P(SearchTest, KolokolExample) {
    auto searcher = createSearcher();
    searcher->setPattern("KOLOKOL");
    // K O L K O K O L O K O L  L
    // 0 1 2 3 4 5 6 7 8 9 10 11 12
    //           K O L O K O L  ← совпадение на позиции 5
    EXPECT_EQ(searcher->search("KOLKOKOLOKOLL"), 5);
}

TEST_P(SearchTest, KolokolSearchAll) {
    auto searcher = createSearcher();
    searcher->setPattern("KOLOKOL");
    auto results = searcher->searchAll("KOLKOKOLOKOLL");
    
    ASSERT_EQ(results.size(), 1u);
    EXPECT_EQ(results[0], 5);
}

// ============================================
// Тесты переиспользования объекта
// ============================================

TEST_P(SearchTest, ReuseWithSetPattern) {
    auto searcher = createSearcher();
    
    // Первый pattern
    searcher->setPattern("ABC");
    EXPECT_EQ(searcher->search("ABCDEF"), 0);
    EXPECT_EQ(searcher->getPattern(), "ABC");
    
    // Второй pattern — объект переиспользуется
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
    
    // setPattern должен сбросить счётчик
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

TEST_P(SearchTest, WorstCaseForNaive) {
    auto searcher = createSearcher();
    
    // Худший случай: "AAAB" в "AAAA...A"
    std::string text(100, 'A');
    searcher->setPattern("AAAB");
    EXPECT_EQ(searcher->search(text), -1);
}

TEST_P(SearchTest, BestCaseForBoyerMoore) {
    auto searcher = createSearcher();
    
    // Лучший случай: образец с уникальными символами, не в тексте
    std::string text(1000, 'A');
    searcher->setPattern("BCDEF");
    EXPECT_EQ(searcher->search(text), -1);
    
    // Boyer-Moore и BMH должны сделать мало сравнений
    // (пропуская по 5 символов за раз)
}

TEST_P(SearchTest, LongPatternInLongText) {
    auto searcher = createSearcher();
    
    std::string text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    text += text;  // 52 символа
    
    searcher->setPattern("MNOPQRST");
    
    int pos = searcher->search(text);
    EXPECT_EQ(pos, 12);
    
    auto all = searcher->searchAll(text);
    ASSERT_EQ(all.size(), 2u);
    EXPECT_EQ(all[0], 12);
    EXPECT_EQ(all[1], 38);  // 12 + 26
}

// ============================================
// Тест getName()
// ============================================

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
        AlgorithmType::Naive,
        AlgorithmType::BMH,
        AlgorithmType::BoyerMoore
    ),
    [](const testing::TestParamInfo<AlgorithmType>& info) {
        return algorithmName(info.param);
    }
);

// ============================================
// Отдельные тесты для конструкторов
// ============================================

TEST(NaiveSearchTest, ConstructorWithPattern) {
    NaiveSearch searcher("ABC");
    EXPECT_EQ(searcher.getPattern(), "ABC");
    EXPECT_EQ(searcher.search("ABCDEF"), 0);
}

TEST(BMHSearchTest, ConstructorWithPattern) {
    BMHSearch searcher("ABC");
    EXPECT_EQ(searcher.getPattern(), "ABC");
    EXPECT_EQ(searcher.search("ABCDEF"), 0);
}

TEST(BoyerMooreSearchTest, ConstructorWithPattern) {
    BoyerMooreSearch searcher("ABC");
    EXPECT_EQ(searcher.getPattern(), "ABC");
    EXPECT_EQ(searcher.search("ABCDEF"), 0);
}

// ============================================
// Тесты таблицы good suffix для Boyer-Moore
// ============================================

TEST(BoyerMooreSearchTest, GoodSuffixKolokol) {
    BoyerMooreSearch searcher("KOLOKOL");
    
    // K O L K O K O L O K O L  L
    // 0 1 2 3 4 5 6 7 8 9 10 11 12
    //           K O L O K O L  ← совпадение на позиции 5
    EXPECT_EQ(searcher.search("KOLKOKOLOKOLL"), 5);
    
    // Проверяем количество сравнений (должно быть меньше, чем у наивного)
    size_t bmComparisons = searcher.getComparisonCount();
    
    NaiveSearch naive("KOLOKOL");
    naive.search("KOLKOKOLOKOLL");
    size_t naiveComparisons = naive.getComparisonCount();
    
    // Boyer-Moore должен сделать не больше сравнений, чем наивный
    EXPECT_LE(bmComparisons, naiveComparisons);
}

TEST(BoyerMooreSearchTest, ComplexPatternSuffix) {
    // Паттерн с повторяющимися подстроками
    BoyerMooreSearch searcher("ABCABC");
    
    EXPECT_EQ(searcher.search("XYZABCABCDEF"), 3);
    
    auto all = searcher.searchAll("ABCABCABCABC");
    ASSERT_EQ(all.size(), 3u);
    EXPECT_EQ(all[0], 0);
    EXPECT_EQ(all[1], 3);
    EXPECT_EQ(all[2], 6);
}
