#include "SingleArray.h"
#include <gtest/gtest.h>

TEST(SingleArrayTest, InitiallyEmpty) {
    SingleArray<int> array;
    EXPECT_EQ(array.size(), 0);
}

TEST(SingleArrayTest, AddElementsToFront) {
    SingleArray<int> array;
    array.add(10, 0);
    array.add(20, 0);
    array.add(30, 0);

    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.get(0), 30);
    EXPECT_EQ(array.get(1), 20);
    EXPECT_EQ(array.get(2), 10);
}

TEST(SingleArrayTest, AddElementsToEnd) {
    SingleArray<int> array;
    array.add(10, 0);
    array.add(20, 1);
    array.add(30, 2);

    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.get(0), 10);
    EXPECT_EQ(array.get(1), 20);
    EXPECT_EQ(array.get(2), 30);
}

TEST(SingleArrayTest, AddElementsToMiddle) {
    SingleArray<int> array;
    array.add(10, 0);  // [10]
    array.add(30, 1);  // [10, 30]
    array.add(20, 1);  // [10, 20, 30]

    EXPECT_EQ(array.get(0), 10);
    EXPECT_EQ(array.get(1), 20);
    EXPECT_EQ(array.get(2), 30);
}

TEST(SingleArrayTest, RemoveElements) {
    SingleArray<int> array;
    array.add(1, 0);
    array.add(2, 1);
    array.add(3, 2);  // [1, 2, 3]

    int removed = array.remove(1);  // Remove 2
    EXPECT_EQ(removed, 2);
    EXPECT_EQ(array.size(), 2);
    EXPECT_EQ(array.get(0), 1);
    EXPECT_EQ(array.get(1), 3);
}

TEST(SingleArrayTest, RemoveFromFrontAndEnd) {
    SingleArray<int> array;
    array.add(1, 0);
    array.add(2, 1);
    array.add(3, 2);

    EXPECT_EQ(array.remove(0), 1); // front
    EXPECT_EQ(array.remove(1), 3); // end
    EXPECT_EQ(array.get(0), 2);
    EXPECT_EQ(array.size(), 1);
}

TEST(SingleArrayTest, GetInvalidIndexThrows) {
    SingleArray<int> array;
    array.add(1, 0);

    EXPECT_THROW(array.get(-1), std::out_of_range);
    EXPECT_THROW(array.get(2), std::out_of_range);
}

TEST(SingleArrayTest, AddInvalidIndexThrows) {
    SingleArray<int> array;
    EXPECT_THROW(array.add(1, -1), std::out_of_range);
    EXPECT_THROW(array.add(1, 2), std::out_of_range);
}

TEST(SingleArrayTest, RemoveInvalidIndexThrows) {
    SingleArray<int> array;
    EXPECT_THROW(array.remove(0), std::out_of_range);
    array.add(42, 0);
    EXPECT_THROW(array.remove(1), std::out_of_range);
}
