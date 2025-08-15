#include <gtest/gtest.h>
#include "Queen.h"

// a1 (позиция 0): слева снизу, как у ладьи + слона
TEST(QueenTest, PositionA1) {
    Queen queen;
    auto result = queen.getMoves(0);
    EXPECT_EQ(result.first, 21);
    EXPECT_EQ(result.second, 0x81412111090503FEULL);
}

// d4 (позиция 27): объединение ходов ладьи и слона
TEST(QueenTest, PositionD4) {
    Queen queen;
    auto result = queen.getMoves(27);
    EXPECT_EQ(result.first, 27);
    EXPECT_EQ(result.second, 9820426766351346249);
}

// h8 (позиция 63): диагональ + горизонталь + вертикаль
TEST(QueenTest, PositionH8) {
    Queen queen;
    auto result = queen.getMoves(63);
    EXPECT_EQ(result.first, 21);
    EXPECT_EQ(result.second, 9205534180971414145);
}

// Проверка некорректной позиции
TEST(QueenTest, InvalidPosition) {
    Queen queen;
    auto result = queen.getMoves(-1);
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 0);
    result = queen.getMoves(64);
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 0);
}
