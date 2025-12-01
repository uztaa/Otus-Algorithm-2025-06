#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>

#include "ILinearSorter.hpp"
#include "CountingSorter.hpp"
#include "RadixSorter.hpp"
#include "BucketSorter.hpp"

/**
 * @brief Проверка отсортированности массива
 */
template<typename T>
bool isSorted(const std::vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Typed test suite для всех сортировщиков
 */
template <typename T>
class LinearSorterTest : public ::testing::Test {
public:
    std::unique_ptr<ILinearSorter> sorter;

    void SetUp() override {
        sorter = std::make_unique<T>();
    }
};

using SorterTypes = ::testing::Types<CountingSorter, RadixSorter, BucketSorter>;

TYPED_TEST_SUITE(LinearSorterTest, SorterTypes);

// ============ Тесты для uint16_t ============

TYPED_TEST(LinearSorterTest, EmptyArray16) {
    std::vector<uint16_t> arr;
    this->sorter->sort(arr);
    EXPECT_TRUE(arr.empty());
}

TYPED_TEST(LinearSorterTest, SingleElement16) {
    std::vector<uint16_t> arr = {42};
    this->sorter->sort(arr);
    ASSERT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 42);
}

TYPED_TEST(LinearSorterTest, TwoElements16) {
    std::vector<uint16_t> arr = {200, 100};
    this->sorter->sort(arr);
    EXPECT_TRUE(isSorted(arr));
    EXPECT_EQ(arr[0], 100);
    EXPECT_EQ(arr[1], 200);
}

TYPED_TEST(LinearSorterTest, SortedArray16) {
    std::vector<uint16_t> arr = {1, 2, 3, 4, 5};
    this->sorter->sort(arr);
    EXPECT_TRUE(isSorted(arr));
}

TYPED_TEST(LinearSorterTest, ReversedArray16) {
    std::vector<uint16_t> arr = {500, 400, 300, 200, 100};
    this->sorter->sort(arr);
    EXPECT_TRUE(isSorted(arr));
    EXPECT_EQ(arr[0], 100);
    EXPECT_EQ(arr[4], 500);
}

TYPED_TEST(LinearSorterTest, Duplicates16) {
    std::vector<uint16_t> arr = {5, 3, 5, 1, 3, 5, 1};
    this->sorter->sort(arr);
    EXPECT_TRUE(isSorted(arr));
}

TYPED_TEST(LinearSorterTest, AllEqual16) {
    std::vector<uint16_t> arr = {777, 777, 777, 777};
    this->sorter->sort(arr);
    EXPECT_TRUE(isSorted(arr));
    for (const auto& v : arr) {
        EXPECT_EQ(v, 777);
    }
}

TYPED_TEST(LinearSorterTest, RandomArray16) {
    std::vector<uint16_t> arr = {234, 12, 890, 45, 678, 1, 999, 456};
    this->sorter->sort(arr);
    EXPECT_TRUE(isSorted(arr));
}

TYPED_TEST(LinearSorterTest, MaxValues16) {
    std::vector<uint16_t> arr = {65535, 0, 32768, 1, 65534};
    this->sorter->sort(arr);
    EXPECT_TRUE(isSorted(arr));
    EXPECT_EQ(arr[0], 0);
    EXPECT_EQ(arr[4], 65535);
}

TYPED_TEST(LinearSorterTest, LargeArray16) {
    std::vector<uint16_t> arr;
    std::mt19937 rng(12345);
    std::uniform_int_distribution<uint16_t> dist(0, 999);
    
    for (int i = 0; i < 10000; ++i) {
        arr.push_back(dist(rng));
    }
    
    this->sorter->sort(arr);
    EXPECT_TRUE(isSorted(arr));
}

// ============ Тесты для uint32_t ============

TYPED_TEST(LinearSorterTest, EmptyArray32) {
    std::vector<uint32_t> arr;
    this->sorter->sort(arr, 0);
    EXPECT_TRUE(arr.empty());
}

TYPED_TEST(LinearSorterTest, RandomArray32_Range999) {
    std::vector<uint32_t> arr = {234, 12, 890, 45, 678, 1, 999, 456, 0};
    this->sorter->sort(arr, 999);
    EXPECT_TRUE(isSorted(arr));
}

TYPED_TEST(LinearSorterTest, LargeArray32) {
    std::vector<uint32_t> arr;
    std::mt19937 rng(54321);
    std::uniform_int_distribution<uint32_t> dist(0, 999);
    
    for (int i = 0; i < 10000; ++i) {
        arr.push_back(dist(rng));
    }
    
    this->sorter->sort(arr, 999);
    EXPECT_TRUE(isSorted(arr));
}

// ============ Индивидуальные тесты ============

TEST(CountingSorterTest, CorrectName) {
    CountingSorter sorter;
    EXPECT_EQ(sorter.getName(), "CountingSorter");
}

TEST(RadixSorterTest, CorrectName) {
    RadixSorter sorter;
    EXPECT_EQ(sorter.getName(), "RadixSorter");
}

TEST(BucketSorterTest, CorrectName) {
    BucketSorter sorter;
    EXPECT_EQ(sorter.getName(), "BucketSorter");
}

TEST(RadixSorterTest, DifferentBases) {
    std::vector<uint16_t> arr1 = {500, 100, 999, 0, 250};
    std::vector<uint16_t> arr2 = arr1;
    
    RadixSorter sorter10(10);
    RadixSorter sorter256(256);
    
    sorter10.sort(arr1);
    sorter256.sort(arr2);
    
    EXPECT_TRUE(isSorted(arr1));
    EXPECT_TRUE(isSorted(arr2));
    EXPECT_EQ(arr1, arr2);
}

TEST(BucketSorterTest, CustomBucketCount) {
    std::vector<uint16_t> arr = {500, 100, 999, 0, 250, 750};
    
    BucketSorter sorter(10);  // 10 корзин
    sorter.sort(arr);
    
    EXPECT_TRUE(isSorted(arr));
}