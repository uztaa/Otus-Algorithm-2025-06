#include <gtest/gtest.h>
#include "BenchmarkDataGenerator.h"
#include <algorithm>

class BenchmarkDataGeneratorTest : public ::testing::Test {
protected:
    BenchmarkDataGenerator gen;
};

TEST_F(BenchmarkDataGeneratorTest, Random_GeneratesCorrectSize) {
    auto data = gen.random(1000);
    ASSERT_EQ(data.size(), 1000);
}

TEST_F(BenchmarkDataGeneratorTest, Digits_GeneratesDigitsOnly) {
    auto data = gen.digits(1000);
    ASSERT_EQ(data.size(), 1000);
    for (auto& r : data) {
        EXPECT_GE(r.getKey(), 0); // проверяем, что весь массив - цифры
        EXPECT_LE(r.getKey(), 9);
    }
}

TEST_F(BenchmarkDataGeneratorTest, Revers_IsDescending) {
    size_t size = 100;
    auto data = gen.revers(size);
    ASSERT_EQ(data.size(), size);
    for (size_t i = 1; i < size; ++i) {
        EXPECT_GE(data[i-1].getKey(), data[i].getKey());
    }
}

TEST_F(BenchmarkDataGeneratorTest, Sorted_AllButDirtiesSorted) {
    size_t size = 1000;
    size_t dirties = size / 100;  // 1% dirty
    auto data = gen.sorted(size, dirties);
    ASSERT_EQ(data.size(), size);

    size_t unsortedCount = 0;
    for (size_t i = 1; i < size; ++i) {
        if (data[i-1].getKey() > data[i].getKey()) {
            ++unsortedCount;
        }
    }
    // Проверяем, что количество перестановок приблизительно равно dirties / 2
    // (каждая перестановка меняет два элемента, но считаем нестрого)
    EXPECT_LE(unsortedCount, dirties);
}
