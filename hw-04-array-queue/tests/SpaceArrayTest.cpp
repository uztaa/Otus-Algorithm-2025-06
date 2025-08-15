#include <gtest/gtest.h>
#include "SpaceArray.h"

TEST(SpaceArrayTest, AddAndGetElements) {
    SpaceArray<int> arr;
    arr.add(0, 10);
    arr.add(0, 20);
    arr.add(1, 100);
    arr.add(2, 200);

    EXPECT_EQ(arr.blockCount(), 3);
    EXPECT_EQ(arr.blockSize(0), 2);
    EXPECT_EQ(arr.blockSize(1), 1);
    EXPECT_EQ(arr.blockSize(2), 1);

    EXPECT_EQ(arr.get(0, 0), 10);
    EXPECT_EQ(arr.get(0, 1), 20);
    EXPECT_EQ(arr.get(1, 0), 100);
    EXPECT_EQ(arr.get(2, 0), 200);
}

TEST(SpaceArrayTest, RemoveElements) {
    SpaceArray<int> arr;
    arr.add(0, 1);
    arr.add(0, 2);
    arr.add(1, 3);

    EXPECT_EQ(arr.remove(0, 0), 1);
    EXPECT_EQ(arr.blockSize(0), 1);
    EXPECT_EQ(arr.get(0, 0), 2);

    EXPECT_EQ(arr.remove(1, 0), 3);
    EXPECT_EQ(arr.blockSize(1), 0);
}

TEST(SpaceArrayTest, OutOfRangeThrows) {
    SpaceArray<int> arr;
    arr.add(0, 1);

    EXPECT_THROW(arr.get(1, 0), std::out_of_range);
    EXPECT_THROW(arr.get(0, 1), std::out_of_range);
    EXPECT_THROW(arr.remove(1, 0), std::out_of_range);
    EXPECT_THROW(arr.remove(0, 1), std::out_of_range);
    EXPECT_THROW(arr.blockSize(2), std::out_of_range);
    EXPECT_THROW(arr.add(-1, 5), std::out_of_range);
}