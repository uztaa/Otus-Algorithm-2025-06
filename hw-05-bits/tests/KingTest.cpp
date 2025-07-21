#include <gtest/gtest.h>
#include "King.h"
#include "BitUtils.h"

TEST(KingTest, PositionA1) {
    King king;
    auto result = king.getMoves(0);
    EXPECT_EQ(result.first, 3);
    EXPECT_EQ(result.second, 770);
}

TEST(KingTest, PositionH1) {
    King king;
    auto result = king.getMoves(7);
    EXPECT_EQ(result.first, 3);
    EXPECT_EQ(result.second, 49216);
}

TEST(KingTest, PositionD4) {
    King king;
    auto result = king.getMoves(27);
    EXPECT_EQ(result.first, 8);
    EXPECT_EQ(BitUtils::popcount(result.second), 8);
}

TEST(KingTest, PositionA8) {
    King king;
    auto result = king.getMoves(56);
    EXPECT_EQ(result.first, 3);
    EXPECT_EQ(result.second, 144959613005987840ULL);
}

TEST(KingTest, PositionH8) {
    King king;
    auto result = king.getMoves(63);
    EXPECT_EQ(result.first, 3);
    EXPECT_EQ(result.second, 4665729213955833856ULL);
}

TEST(KingTest, InvalidPosition) {
    King king;
    auto result = king.getMoves(-1);
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 0);

    result = king.getMoves(64);
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 0);
}