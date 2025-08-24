#include <gtest/gtest.h>
#include <gtest/gtest-typed-test.h>

#include <string>
#include "utils/SortTestUtils.h"
#include "BaseSorter.h"

#include "QuickSorter.h"
#include "MergeSorter.h"
#include "CocktailSorter.h"
#include "BinaryInsertionSorter.h"
#include "LoggingSortListener.h"
#include "InsertionSorter.h"

template <typename T>
class SorterTest : public ::testing::Test {
public:
    std::unique_ptr<BaseSorter> sorter;

    void SetUp() override {
        sorter = std::make_unique<T>();

        std::string s(typeid(T).name());
		sorter->setName(s.erase(0, 6));

        // для дебага раскомментировать
        //sorter->addListener(std::make_shared<LoggingSortListener>());
    }
};

using SorterTypes = ::testing::Types<
    MergeSorter,
    QuickSorter,
    BinaryInsertionSorter,
    CocktailSorter,
    InsertionSorter
>;

TYPED_TEST_SUITE(SorterTest, SorterTypes);

TYPED_TEST(SorterTest, RandomSortedArray) {
    std::vector<Record> data = {
        {24, "a"}, {89, "b"}, {66, "c"}, {72, "d"}, {36, "e"}, {89, "f"}, {43, "g"}, {83, "h"}, {81, "i"}, {12, "j"}
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
    // TODO: EXPECT_EQ(shifts, 0);
}

TYPED_TEST(SorterTest, HandlesReverseArray) {
    std::vector<Record> data = {
        Record(5, "e"), Record(4, "d"),
        Record(3, "c"), Record(2, "b"), Record(1, "a")
    };
    
    this->sorter->sort(data);
    
    EXPECT_TRUE(isSorted(data));
}

TYPED_TEST(SorterTest, HandlesDuplicates) {
    std::vector<Record> data = {
        Record(3, "a"), Record(1, "b"),
        Record(3, "c"), Record(1, "d"), Record(2, "e")
    };
    
    this->sorter->sort(data);
    
    EXPECT_TRUE(isSorted(data));
}

TYPED_TEST(SorterTest, HandlesAllEqualElements) {
    std::vector<Record> data = {
        Record(5, "a"), Record(5, "b"),
        Record(5, "c"), Record(5, "d")
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
    std::vector<Record> data = { Record(42, "x") };
    
    this->sorter->sort(data);
    
    ASSERT_EQ(data.size(), 1);
    ASSERT_EQ(data[0].getKey(), 42);
}
