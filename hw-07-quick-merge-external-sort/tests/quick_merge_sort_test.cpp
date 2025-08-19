#include <gtest/gtest.h>
#include "QuickSorter.h"
#include "MergeSorter.h"
#include "RandomArrayGenerator.h"

// Проверка, что массив отсортирован по возрастанию ключей
bool isSortedByKey(const std::vector<Record>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1].getKey() > arr[i].getKey()) return false;
    }
    return true;
}

class SorterTest : public ::testing::Test {
protected:
    RandomArrayGenerator generator;
    constexpr static size_t size = 100;
    constexpr static uint32_t seed = 12345;  // фиксированный сид для воспроизводимости
};

// Тест для QuickSorter с фиксированными данными из regenerate
TEST_F(SorterTest, QuickSorter_SortsCorrectly_WithFixedSeed) {
    auto data = generator.regenerate(size, seed);

    QuickSorter sorter;
    sorter.sort(data);

    EXPECT_TRUE(isSortedByKey(data));
}

// Тест для MergeSorter с фиксированными данными из regenerate
TEST_F(SorterTest, MergeSorter_SortsCorrectly_WithFixedSeed) {
    auto data = generator.regenerate(size, seed);

    MergeSorter sorter;
    sorter.sort(data);

    EXPECT_TRUE(isSortedByKey(data));
}

// Проверка на уже отсортированных данных для QuickSorter
TEST_F(SorterTest, QuickSorter_AlreadySorted_WithFixedSeed) {
    auto data = generator.regenerate(size, seed);
    MergeSorter().sort(data);  // предварительно отсортировать

    QuickSorter sorter;
    sorter.sort(data);

    EXPECT_TRUE(isSortedByKey(data));
}

// Проверка на уже отсортированных данных для MergeSorter
TEST_F(SorterTest, MergeSorter_AlreadySorted_WithFixedSeed) {
    auto data = generator.regenerate(size, seed);
    QuickSorter().sort(data);  // предварительно отсортировать

    MergeSorter sorter;
    sorter.sort(data);

    EXPECT_TRUE(isSortedByKey(data));
}
