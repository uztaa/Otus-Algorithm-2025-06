#include <gtest/gtest.h>
#include <gtest/gtest-typed-test.h>

#include <string>
#include "utils/SortTestUtils.h"
#include "BaseSorter.h"

// Подключи все сортировщики:
#include "../src/sort/BubbleSorter.cpp"
#include "../src/sort/CocktailSorter.cpp"
#include "../src/sort/InsertionShiftSorter.cpp"
#include "../src/sort/InsertionSorter.cpp"
#include "../src/sort/BinaryInsertionSorter.cpp"
#include "../src/sort/LoggingSortListener.cpp"

// Шаблонный тест-класс
template <typename T>
class SorterTest : public ::testing::Test {
public:
    std::unique_ptr<BaseSorter> sorter;

    void SetUp() override {
        sorter = std::make_unique<T>();

		// Устанавливаем имя сортировщика для логирования
        std::string s(typeid(T).name());
		sorter->setName(s.erase(0, 6)); // удаляем 'class '

        // Добавляем слушателя для логирования
        sorter->addListener(std::make_shared<LoggingSortListener>());
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

TYPED_TEST(SorterTest, ProblemSortedArray) {
    // [24, 89, 66, 72, 36, 89, 43, 83, 81, 12]
    std::vector<Record> data = {
        {24, "a"}, {89, "b"}, {66, "c"}, {72, "d"}, {36, "e"}, {89, "f"}, {43, "g"}, {83, "h"}, {81, "i"}, {12, "j"}
    };
    auto [comparisons, shifts] = this->sorter->sort(data);
    EXPECT_TRUE(isSorted(data));
}

// Проверка на массив, который уже отсортирован
TYPED_TEST(SorterTest, HandlesSortedArray) {
    std::vector<Record> data = {
        {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}
    };
    auto [comparisons, shifts] = this->sorter->sort(data);
    EXPECT_TRUE(isSorted(data));
    EXPECT_EQ(shifts, 0);
}

// Проверка массива, который отсортирован в обратном порядке
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
