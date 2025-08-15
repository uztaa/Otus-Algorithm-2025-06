#include <gtest/gtest.h>
#include "Bishop.h"

// a1 (позиция 0): вверх-вправо по диагонали (b2, c3, ..., h8)
TEST(BishopTest, PositionA1) {
    Bishop bishop;
    auto result = bishop.getMoves(0);
    EXPECT_EQ(result.first, 7);
    EXPECT_EQ(result.second, 0x8040201008040200ULL);
}

// d4 (позиция 27): все диагонали
TEST(BishopTest, PositionD4) {
    Bishop bishop;
    auto result = bishop.getMoves(27);
    EXPECT_EQ(result.first, 13);
    EXPECT_EQ(result.second, 9241705379636978241);
}

// h8 (позиция 63): вниз-влево по диагонали
TEST(BishopTest, PositionH8) {
    Bishop bishop;
    auto result = bishop.getMoves(63);
    EXPECT_EQ(result.first, 7);
    EXPECT_EQ(result.second, 0x40201008040201ULL);
}

// Проверка некорректной позиции
TEST(BishopTest, InvalidPosition) {
    Bishop bishop;
    auto result = bishop.getMoves(-1);
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 0);
    result = bishop.getMoves(64);
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 0);
}
