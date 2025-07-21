#include <gtest/gtest.h>
#include "FastBitUtils.h"

TEST(FastBitUtilsTest, PopcountZero) {
    EXPECT_EQ(FastBitUtils::popcount(0), 0);
}

TEST(FastBitUtilsTest, PopcountAllBitsSet) {
    EXPECT_EQ(FastBitUtils::popcount(UINT64_MAX), 64);
}

TEST(FastBitUtilsTest, PopcountSingleBits) {
    EXPECT_EQ(FastBitUtils::popcount(1ULL), 1);               // 000...0001
    EXPECT_EQ(FastBitUtils::popcount(1ULL << 63), 1);         // 100...0000
}

TEST(FastBitUtilsTest, PopcountRandomBits) {
    EXPECT_EQ(FastBitUtils::popcount(0b101010), 3);           // 3 бита
    EXPECT_EQ(FastBitUtils::popcount(0xF0F0F0F0F0F0F0F0ULL), 32);
}
