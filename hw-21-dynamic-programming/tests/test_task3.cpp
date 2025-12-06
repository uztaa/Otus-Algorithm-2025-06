#include <gtest/gtest.h>
#include "Task3_FiveEight.hpp"

class Task3FiveEightTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// N=1: только "5" и "8"
TEST_F(Task3FiveEightTest, N1) {
    long long result = TaskTheFiveEight::execute(1);
    EXPECT_EQ(result, 2);  // "5", "8"
}

// N=2: "55", "58", "85", "88"
TEST_F(Task3FiveEightTest, N2) {
    long long result = TaskTheFiveEight::execute(2);
    EXPECT_EQ(result, 4);
}

// N=3: исключаем "555" и "888" из 8 вариантов
TEST_F(Task3FiveEightTest, N3) {
    long long result = TaskTheFiveEight::execute(3);
    EXPECT_EQ(result, 6);  // "558", "585", "588", "855", "858", "885"
}

// N=4
TEST_F(Task3FiveEightTest, N4) {
    long long result = TaskTheFiveEight::execute(4);
    EXPECT_GT(result, 6);
    EXPECT_LT(result, 100);
}

// N=5
TEST_F(Task3FiveEightTest, N5) {
    long long result = TaskTheFiveEight::execute(5);
    EXPECT_GT(result, 0);
}

// Ноль не определён или равен 0
TEST_F(Task3FiveEightTest, N0) {
    long long result = TaskTheFiveEight::execute(0);
    EXPECT_EQ(result, 0);
}

// Отрицательное N
TEST_F(Task3FiveEightTest, NegativeN) {
    long long result = TaskTheFiveEight::execute(-1);
    EXPECT_EQ(result, 0);
}

// Последовательность растёт
TEST_F(Task3FiveEightTest, SequenceGrows) {
    long long r1 = TaskTheFiveEight::execute(1);
    long long r2 = TaskTheFiveEight::execute(2);
    long long r3 = TaskTheFiveEight::execute(3);
    EXPECT_LT(r1, r2);
    EXPECT_LT(r2, r3);
}

// Большое N (не должно переполниться сразу)
TEST_F(Task3FiveEightTest, LargeN) {
    long long result = TaskTheFiveEight::execute(10);
    EXPECT_GT(result, 0);
}
