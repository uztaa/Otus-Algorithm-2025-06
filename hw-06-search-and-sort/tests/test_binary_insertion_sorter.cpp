#include <gtest/gtest.h>
#include <vector>
#include "Record.h"
#include "Sortable.h"

// Подключаем тестируемый класс
#include "../src/sort/BinaryInsertionSorter.cpp" // Или подключите .h-файл, если он у вас есть

// Вспомогательная функция для проверки отсортированного массива
bool isSorted(const std::vector<Record>& arr) {
    for (size_t i = 1; i < arr.size(); ++i)
        if (arr[i - 1].getKey() > arr[i].getKey())
            return false;
    return true;
}

// Вспомогательная функция для печати вектора
void printRecords(const std::vector<Record>& data) {
    std::cout << "Sort result: ";
    for (const auto& record : data) {
        std::cout << "(" << record.getKey() << ", " << record.getValue() << ") ";
    }
    std::cout << std::endl;
}

// Проверка уже отсортированного массива
TEST(BinaryInsertionSorterTest, HandlesSortedArray) {
    std::vector<Record> data = {
        Record(1, "a"),
        Record(2, "b"),
        Record(3, "c"),
        Record(4, "d"),
        Record(5, "e")
    };

    BinaryInsertionSorter sorter;
    auto [comparisons, shifts] = sorter.sort(data);

    // Выводим в лог содержимое массива после сортировки
    printRecords(data);

    EXPECT_TRUE(isSorted(data));
    EXPECT_EQ(shifts, 0); // Не должно быть сдвигов
}

/*

// Проверка пустого массива
TEST(BinaryInsertionSorterTest, HandlesEmptyArray) {
    std::vector<Record> data;
    BinaryInsertionSorter sorter;
    auto [comparisons, shifts] = sorter.sort(data);

    EXPECT_TRUE(data.empty());
    EXPECT_EQ(comparisons, 0);
    EXPECT_EQ(shifts, 0);
}

// Проверка массива из одного элемента
TEST(BinaryInsertionSorterTest, HandlesSingleElement) {
    std::vector<Record> data = { Record(5, "a")};
    BinaryInsertionSorter sorter;
    auto [comparisons, shifts] = sorter.sort(data);

    EXPECT_EQ(data.size(), 1);
    EXPECT_EQ(data[0].getKey(), 5);
    EXPECT_EQ(comparisons, 0); // Нет смысла искать
    EXPECT_EQ(shifts, 0);      // Ничего не сдвигается
}



// Проверка массива в обратном порядке
TEST(BinaryInsertionSorterTest, HandlesReverseArray) {
    std::vector<Record> data = { Record(5, "e"), Record(4, "d"), Record(3, "c"), Record(2, "b"), Record(1, "a")};
    BinaryInsertionSorter sorter;
    auto [comparisons, shifts] = sorter.sort(data);

    EXPECT_TRUE(isSorted(data));
    EXPECT_EQ(shifts, 10); // Для n=5, максимум (n(n-1)/2) = 10 сдвигов
}

// Проверка массива с повторяющимися значениями
TEST(BinaryInsertionSorterTest, HandlesDuplicates) {
    std::vector<Record> data = { Record(3, "c"), Record(1, "a"), Record(2, "b"), Record(3, "c"), Record(1, "a")};
    BinaryInsertionSorter sorter;
    auto [comparisons, shifts] = sorter.sort(data);

    EXPECT_TRUE(isSorted(data));
    // Проверяем, что элементы с одинаковыми ключами остаются в порядке добавления
    EXPECT_EQ(data[0].getKey(), 1);
    EXPECT_EQ(data[1].getKey(), 1);
    EXPECT_EQ(data[2].getKey(), 2);
    EXPECT_EQ(data[3].getKey(), 3);
    EXPECT_EQ(data[4].getKey(), 3);
}

// Проверка массива с одинаковыми значениями
TEST(BinaryInsertionSorterTest, HandlesAllEqualElements) {
    std::vector<Record> data = { Record(7, "g"), Record(7, "g"), Record(7, "g"), Record(7, "g"), Record(7, "g")};
    BinaryInsertionSorter sorter;
    auto [comparisons, shifts] = sorter.sort(data);

    EXPECT_TRUE(isSorted(data));
    EXPECT_EQ(shifts, 0); // Элементы не двигаются
}


*/