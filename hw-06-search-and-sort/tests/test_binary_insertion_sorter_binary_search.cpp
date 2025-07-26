// Специальный тест на проверку сортировки с использованием бинарного поиска
#include <gtest/gtest.h>
#include "Record.h"
#include "../include/BinaryInsertionSorter.h"

// Тестовый класс для доступа к protected методу binarySearch
class BinaryInsertionSorterTestable : public BinaryInsertionSorter {
public:
    using BinaryInsertionSorter::binarySearch;
};

// Вставка в начало массива
TEST(BinaryInsertionSorterTest, InsertAtBeginning) {
    BinaryInsertionSorterTestable sorter;
    std::vector<Record> arr = {
        Record(20, "a"), Record(40, "b"), Record(60, "c")
    };
    size_t comparisons = 0;
    int pos = sorter.binarySearch(arr, Record(10, "x"), 0, arr.size() - 1, comparisons);
    EXPECT_EQ(pos, 0);
}

// Вставка в конец массива
TEST(BinaryInsertionSorterTest, InsertAtEnd) {
    BinaryInsertionSorterTestable sorter;
    std::vector<Record> arr = {
        Record(20, "a"), Record(40, "b"), Record(60, "c")
    };
    size_t comparisons = 0;
    int pos = sorter.binarySearch(arr, Record(70, "x"), 0, arr.size() - 1, comparisons);
    EXPECT_EQ(pos, 3);
}

// Вставка в середину массива
TEST(BinaryInsertionSorterTest, InsertInMiddle) {
    BinaryInsertionSorterTestable sorter;
    std::vector<Record> arr = {
        Record(20, "a"), Record(40, "b"), Record(60, "c")
    };
    size_t comparisons = 0;
    int pos = sorter.binarySearch(arr, Record(50, "x"), 0, arr.size() - 1, comparisons);
    EXPECT_EQ(pos, 2);
}

// Вставка дубликата ключа — должен вставиться после всех таких ключей
TEST(BinaryInsertionSorterTest, InsertDuplicate) {
    BinaryInsertionSorterTestable sorter;
    std::vector<Record> arr = {
        Record(1, "a"), Record(2, "b"), Record(2, "c"), Record(3, "d")
    };
    size_t comparisons = 0;
    int pos = sorter.binarySearch(arr, Record(2, "x"), 0, arr.size() - 1, comparisons);

	// Ожидаем вставку перед существующим дубликатом
    EXPECT_EQ(pos, 1);
}

// Проверка уже отсортированного массива
TEST(BinaryInsertionSorterTest, ReturnCurrentPositionInSortedArray) {
    BinaryInsertionSorterTestable sorter;
    std::vector<Record> arr = {
        Record(1, "a"),
        Record(2, "b"),
        Record(3, "c"),
        Record(4, "d"),
        Record(5, "e")
    };
    size_t comparisons = 0;
    size_t pos = sorter.binarySearch(arr, Record(3, "c"), 0, arr.size() - 1, comparisons);

    // Ожидаем возвращение позиции = 1
    EXPECT_EQ(pos, 2);
}
