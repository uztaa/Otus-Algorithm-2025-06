#include <gtest/gtest.h>
#include <string>
#include <memory>
#include "BaseSorter.h"
#include "SelectionSorter.h"
#include "HeapSorter.h"
#include "LoggingSortListener.h"

/**
 * @brief Utility function to check if array is sorted
 */
bool isSorted(const std::vector<Record>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1].getKey() > arr[i].getKey()) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Typed test suite for all sorters
 */
template <typename T>
class SorterTest : public ::testing::Test {
public:
    std::unique_ptr<BaseSorter> sorter;

    void SetUp() override {
        sorter = std::make_unique<T>();

        // Set name based on type
        std::string typeName(typeid(T).name());
        // Remove digits from the beginning (compiler-specific mangling)
        size_t i = 0;
        while (i < typeName.length() && std::isdigit(typeName[i])) {
            ++i;
        }
        sorter->setName(typeName.substr(i));

        // Uncomment for debugging
        // sorter->addListener(std::make_shared<LoggingSortListener>());
    }
};

using SorterTypes = ::testing::Types<
    SelectionSorter,
    HeapSorter
>;

TYPED_TEST_SUITE(SorterTest, SorterTypes);

TYPED_TEST(SorterTest, RandomArray) {
    std::vector<Record> data = {
        {24, "a"}, {89, "b"}, {66, "c"}, {72, "d"}, {36, "e"}, 
        {89, "f"}, {43, "g"}, {83, "h"}, {81, "i"}, {12, "j"}
    };
    
    this->sorter->sort(data);
    
    EXPECT_TRUE(isSorted(data));
}

TYPED_TEST(SorterTest, HandlesSortedArray) {
    std::vector<Record> data = {
        {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}
    };
    
    this->sorter->sort(data);
    
    EXPECT_TRUE(isSorted(data));
}

TYPED_TEST(SorterTest, HandlesReverseArray) {
    std::vector<Record> data = {
        {5, "e"}, {4, "d"}, {3, "c"}, {2, "b"}, {1, "a"}
    };
    
    this->sorter->sort(data);
    
    EXPECT_TRUE(isSorted(data));
}

TYPED_TEST(SorterTest, HandlesDuplicates) {
    std::vector<Record> data = {
        {3, "a"}, {1, "b"}, {3, "c"}, {1, "d"}, {2, "e"}
    };
    
    this->sorter->sort(data);
    
    EXPECT_TRUE(isSorted(data));
}

TYPED_TEST(SorterTest, HandlesAllEqualElements) {
    std::vector<Record> data = {
        {5, "a"}, {5, "b"}, {5, "c"}, {5, "d"}
    };
    
    this->sorter->sort(data);
    
    EXPECT_TRUE(isSorted(data));
}

TYPED_TEST(SorterTest, HandlesEmptyArray) {
    std::vector<Record> data;
    
    this->sorter->sort(data);
    
    EXPECT_TRUE(data.empty());
}

TYPED_TEST(SorterTest, HandlesSingleElement) {
    std::vector<Record> data = { {42, "x"} };
    
    this->sorter->sort(data);
    
    ASSERT_EQ(data.size(), 1);
    ASSERT_EQ(data[0].getKey(), 42);
}

TYPED_TEST(SorterTest, HandlesTwoElements) {
    std::vector<Record> data = { {2, "b"}, {1, "a"} };
    
    this->sorter->sort(data);
    
    EXPECT_TRUE(isSorted(data));
    EXPECT_EQ(data[0].getKey(), 1);
    EXPECT_EQ(data[1].getKey(), 2);
}

TYPED_TEST(SorterTest, HandlesLargeArray) {
    std::vector<Record> data;
    // Create array with 1000 elements in reverse order
    for (int i = 1000; i > 0; --i) {
        data.emplace_back(i, "val" + std::to_string(i));
    }
    
    this->sorter->sort(data);
    
    EXPECT_TRUE(isSorted(data));
    EXPECT_EQ(data[0].getKey(), 1);
    EXPECT_EQ(data[999].getKey(), 1000);
}

// Individual tests for specific sorters

TEST(SelectionSorterTest, CorrectName) {
    SelectionSorter sorter;
    EXPECT_EQ(sorter.getName(), "SelectionSorter");
}

TEST(HeapSorterTest, CorrectName) {
    HeapSorter sorter;
    EXPECT_EQ(sorter.getName(), "HeapSorter");
}

// Test to ensure Selection Sort handles worst case (reverse sorted)
TEST(SelectionSorterTest, ReverseOrderedArray) {
    std::vector<Record> data;
    for (int i = 100; i > 0; --i) {
        data.emplace_back(i, "v");
    }
    
    SelectionSorter sorter;
    sorter.sort(data);
    
    EXPECT_TRUE(isSorted(data));
}

// Test to ensure Heap Sort handles heap property correctly
TEST(HeapSorterTest, HeapPropertyMaintained) {
    std::vector<Record> data = {
        {10, "a"}, {20, "b"}, {15, "c"}, {30, "d"}, {40, "e"}
    };
    
    HeapSorter sorter;
    sorter.sort(data);
    
    EXPECT_TRUE(isSorted(data));
    EXPECT_EQ(data[0].getKey(), 10);
    EXPECT_EQ(data[4].getKey(), 40);
}