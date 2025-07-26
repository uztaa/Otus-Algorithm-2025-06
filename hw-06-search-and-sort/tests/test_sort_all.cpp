#include <gtest/gtest.h>
#include <gtest/gtest-typed-test.h>

#include "utils/SortTestUtils.h"
#include "Sortable.h"

// Подключи все сортировщики:
#include "../src/sort/BubbleSorter.cpp"
#include "../src/sort/CocktailSorter.cpp"
#include "../src/sort/InsertionShiftSorter.cpp"
#include "../src/sort/InsertionSorter.cpp"
#include "../src/sort/BinaryInsertionSorter.cpp"

// Шаблонный тест-класс
template <typename T>
class SorterTest : public ::testing::Test {
public:
    std::unique_ptr<Sortable> sorter;

    void SetUp() override {
        sorter = std::make_unique<T>();
    }
};

using SorterTypes = ::testing::Types<
    BubbleSorter,
    CocktailSorter,
    InsertionShiftSorter,
    InsertionSorter,
    BinaryInsertionSorter
>;

TYPED_TEST_SUITE(SorterTest, SorterTypes);

// Пример одного теста
TYPED_TEST(SorterTest, HandlesSortedArray) {
    std::vector<Record> data = {
        {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}
    };
    auto [comparisons, shifts] = this->sorter->sort(data);
    EXPECT_TRUE(isSorted(data));
    EXPECT_EQ(shifts, 0);
}

// Проверка на массив, который уже отсортирован
TYPED_TEST(SorterTest, HandlesReverseArray) {
    std::vector<Record> data = {
        Record(5, "e"), Record(4, "d"),
        Record(3, "c"), Record(2, "b"), Record(1, "a")
    };
    auto [comparisons, shifts] = this->sorter->sort(data);
    EXPECT_TRUE(isSorted(data));
}

// Проверка на массив с дубликатами
TYPED_TEST(SorterTest, HandlesDuplicates) {
    std::vector<Record> data = {
        Record(3, "a"), Record(1, "b"),
        Record(3, "c"), Record(1, "d"), Record(2, "e")
    };
    auto [comparisons, shifts] = this->sorter->sort(data);
    EXPECT_TRUE(isSorted(data));
}

// Проверка на массив с одинаковыми элементами
TYPED_TEST(SorterTest, HandlesAllEqualElements) {
    std::vector<Record> data = {
        Record(5, "a"), Record(5, "b"),
        Record(5, "c"), Record(5, "d")
    };
    auto [comparisons, shifts] = this->sorter->sort(data);
    EXPECT_TRUE(isSorted(data));
}

// Проверка на пустой массив
TYPED_TEST(SorterTest, HandlesEmptyArray) {
    std::vector<Record> data;
    auto [comparisons, shifts] = this->sorter->sort(data);
    EXPECT_TRUE(data.empty());
}

// Проверка на массив из одного элемента
TYPED_TEST(SorterTest, HandlesSingleElement) {
    std::vector<Record> data = { Record(42, "x") };
    auto [comparisons, shifts] = this->sorter->sort(data);
    ASSERT_EQ(data.size(), 1);
    ASSERT_EQ(data[0].getKey(), 42);
}
