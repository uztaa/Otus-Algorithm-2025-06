#include "MatrixArray.h"
#include <gtest/gtest.h>

TEST(MatrixArrayTest, InitiallyEmpty) {
    MatrixArray<int> array;
    EXPECT_EQ(array.size(), 0);
}

TEST(MatrixArrayTest, AddElementsToFront) {
    MatrixArray<int> array;
    array.add(10, 0);
    array.add(20, 0);
    array.add(30, 0);

    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.get(0), 30);
    EXPECT_EQ(array.get(1), 20);
    EXPECT_EQ(array.get(2), 10);
}

TEST(MatrixArrayTest, AddElementsToEnd) {
    MatrixArray<int> array;
    array.add(10, 0);
    array.add(20, 1);
    array.add(30, 2);

    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.get(0), 10);
    EXPECT_EQ(array.get(1), 20);
    EXPECT_EQ(array.get(2), 30);
}

TEST(MatrixArrayTest, AddElementsToMiddle) {
    MatrixArray<int> array;
    array.add(10, 0);  // [10]
    array.add(30, 1);  // [10, 30]
    array.add(20, 1);  // [10, 20, 30]

    EXPECT_EQ(array.get(0), 10);
    EXPECT_EQ(array.get(1), 20);
    EXPECT_EQ(array.get(2), 30);
}

TEST(MatrixArrayTest, RemoveElements) {
    MatrixArray<int> array;
    array.add(1, 0);
    array.add(2, 1);
    array.add(3, 2);  // [1, 2, 3]

    int removed = array.remove(1);  // Remove 2
    EXPECT_EQ(removed, 2);
    EXPECT_EQ(array.size(), 2);
    EXPECT_EQ(array.get(0), 1);
    EXPECT_EQ(array.get(1), 3);
}

TEST(MatrixArrayTest, RemoveFromFrontAndEnd) {
    MatrixArray<int> array;
    array.add(1, 0);
    array.add(2, 1);
    array.add(3, 2);

    EXPECT_EQ(array.remove(0), 1); // front
    EXPECT_EQ(array.remove(1), 3); // end
    EXPECT_EQ(array.get(0), 2);
    EXPECT_EQ(array.size(), 1);
}

TEST(MatrixArrayTest, GetInvalidIndexThrows) {
    MatrixArray<int> array;
    array.add(1, 0);

    EXPECT_THROW(array.get(-1), std::out_of_range);
    EXPECT_THROW(array.get(2), std::out_of_range);
}

TEST(MatrixArrayTest, AddInvalidIndexThrows) {
    MatrixArray<int> array;
    EXPECT_THROW(array.add(1, -1), std::out_of_range);
    EXPECT_THROW(array.add(1, 2), std::out_of_range);
}

TEST(MatrixArrayTest, RemoveInvalidIndexThrows) {
    MatrixArray<int> array;
    EXPECT_THROW(array.remove(0), std::out_of_range);
    array.add(42, 0);
    EXPECT_THROW(array.remove(1), std::out_of_range);
}

TEST(MatrixArrayTest, AddAcrossBlocks) {
    MatrixArray<int> array;

    // Заполняем больше одного блока (например, blockSize = 10)
    for (int i = 0; i < 25; ++i) {
        array.add(i, i);
    }

    EXPECT_EQ(array.size(), 25);
    EXPECT_EQ(array.get(0), 0);
    EXPECT_EQ(array.get(10), 10);
    EXPECT_EQ(array.get(24), 24);
}

TEST(MatrixArrayTest, InsertMiddle) {
    MatrixArray<int> array;
    for (int i = 0; i < 5; ++i) {
        array.add(i * 10, i);
    }

    array.add(25, 2); // вставка между 10 и 20

    EXPECT_EQ(array.get(0), 0);
    EXPECT_EQ(array.get(1), 10);
    EXPECT_EQ(array.get(2), 25);
    EXPECT_EQ(array.get(3), 20);
    EXPECT_EQ(array.get(5), 40);
}

TEST(MatrixArrayTest, RemoveFromMiddle) {
    MatrixArray<int> array;
    for (int i = 0; i < 5; ++i) {
        array.add(i, i);
    }

    int removed = array.remove(2);
    EXPECT_EQ(removed, 2);
    EXPECT_EQ(array.get(2), 3);
    EXPECT_EQ(array.size(), 4);
}

TEST(MatrixArrayTest, OutOfBounds) {
    MatrixArray<int> array;
    EXPECT_THROW(array.get(0), std::out_of_range);
    EXPECT_THROW(array.remove(0), std::out_of_range);
    EXPECT_THROW(array.add(1, 1), std::out_of_range);
}
