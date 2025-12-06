#include <gtest/gtest.h>
#include "Task4_Islands.hpp"

class Task4IslandsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Базовый пример из ДЗ
TEST_F(Task4IslandsTest, BasicExample) {
    std::vector<std::vector<int>> matrix = {
        {1, 0, 1},
        {0, 1, 1},
        {1, 0, 1}
    };
    int result = TaskFourIslands::execute(matrix);
    EXPECT_EQ(result, 3);  // Три отдельных острова
}

// Один остров - весь прямоугольник
TEST_F(Task4IslandsTest, OneIsland) {
    std::vector<std::vector<int>> matrix = {
        {1, 1},
        {1, 1}
    };
    int result = TaskFourIslands::execute(matrix);
    EXPECT_EQ(result, 1);
}

// Чередование - каждая единица отдельный остров
TEST_F(Task4IslandsTest, AllSeparate) {
    std::vector<std::vector<int>> matrix = {
        {1, 0, 1},
        {0, 1, 0},
        {1, 0, 1}
    };
    int result = TaskFourIslands::execute(matrix);
    EXPECT_EQ(result, 5);  // Пять единиц, каждая отдельно
}

// Только нули
TEST_F(Task4IslandsTest, AllZeros) {
    std::vector<std::vector<int>> matrix = {
        {0, 0},
        {0, 0}
    };
    int result = TaskFourIslands::execute(matrix);
    EXPECT_EQ(result, 0);
}

// Только единицы
TEST_F(Task4IslandsTest, AllOnes) {
    std::vector<std::vector<int>> matrix = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    int result = TaskFourIslands::execute(matrix);
    EXPECT_EQ(result, 1);
}

// Два отдельных острова
TEST_F(Task4IslandsTest, TwoIslands) {
    std::vector<std::vector<int>> matrix = {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 1, 1},
        {0, 0, 1, 1}
    };
    int result = TaskFourIslands::execute(matrix);
    EXPECT_EQ(result, 2);
}

// Одна строка с одним островом
TEST_F(Task4IslandsTest, SingleRow) {
    std::vector<std::vector<int>> matrix = {{1, 1, 1}};
    int result = TaskFourIslands::execute(matrix);
    EXPECT_EQ(result, 1);
}

// Один столбец с двумя отдельными островами (разделены нулём)
TEST_F(Task4IslandsTest, SingleColumnTwoIslands) {
    std::vector<std::vector<int>> matrix = {
        {1},
        {0},
        {1}
    };
    int result = TaskFourIslands::execute(matrix);
    // Две 1 разделены 0 - это ДОЛЖНЫ быть 2 острова
    EXPECT_EQ(result, 2);
}

// L-образный остров
TEST_F(Task4IslandsTest, LShapedIsland) {
    std::vector<std::vector<int>> matrix = {
        {1, 0},
        {1, 0},
        {1, 1}
    };
    int result = TaskFourIslands::execute(matrix);
    EXPECT_EQ(result, 1);
}

// Один квадрат 1x1
TEST_F(Task4IslandsTest, SingleCell) {
    std::vector<std::vector<int>> matrix = {{1}};
    int result = TaskFourIslands::execute(matrix);
    EXPECT_EQ(result, 1);
}
