#include <gtest/gtest.h>
#include "BitUtils.h"

TEST(BitUtilsTest, PopcountZero) {
    EXPECT_EQ(BitUtils::popcount(0), 0);
}

TEST(BitUtilsTest, PopcountOne) {
    EXPECT_EQ(BitUtils::popcount(1), 1);
}

TEST(BitUtilsTest, PopcountAllBits) {
    EXPECT_EQ(BitUtils::popcount(0xFFFFFFFFFFFFFFFFULL), 64);
}

TEST(BitUtilsTest, PopcountSomeBits) {
    EXPECT_EQ(BitUtils::popcount(0b101010), 3);
    EXPECT_EQ(BitUtils::popcount(0b1000000000000001ULL), 2);
    EXPECT_EQ(BitUtils::popcount(0xF0F0F0F0F0F0F0F0ULL), 32);
}