#include <gtest/gtest.h>
#include "Rook.h"

// a1 (позиция 0): вертикаль вверх, горизонталь вправо
TEST(RookTest, PositionA1) {
    Rook rook;
    auto result = rook.getMoves(0);
    EXPECT_EQ(result.first, 14);
    // Маска: вертикаль a2-a8 + горизонталь b1-h1
    EXPECT_EQ(result.second, 0x01010101010101FEULL);
}

// d4 (позиция 27): вертикали d1-d8, горизонтали a4-h4 (без самой d4)
TEST(RookTest, PositionD4) {
    Rook rook;
    auto result = rook.getMoves(27);
    EXPECT_EQ(result.first, 14);
    // Ожидаемая маска: вертикаль + горизонталь, кроме самой клетки
    EXPECT_EQ(result.second, 578721386714368008);
}

// h8 (позиция 63): вниз по вертикали h1–h7, влево по горизонтали g8–a8
TEST(RookTest, PositionH8) {
    Rook rook;
    auto result = rook.getMoves(63);
    EXPECT_EQ(result.first, 14);
    EXPECT_EQ(result.second, 9187484529235886208);
}

// Проверка некорректной позиции
TEST(RookTest, InvalidPosition) {
    Rook rook;
    auto result = rook.getMoves(-1);
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 0);
    result = rook.getMoves(64);
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 0);
}

// Проверка позиции на краю доски
TEST(RookTest, PositionA8) {
    Rook rook;
    auto result = rook.getMoves(56);
    EXPECT_EQ(result.first, 14);
    // Маска: вертикаль a1-a7 + горизонталь b8-h8
    EXPECT_EQ(result.second, 18302911464433844481);
}