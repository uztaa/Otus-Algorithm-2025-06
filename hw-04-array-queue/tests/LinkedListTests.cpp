#include "LinkedList.h"
#include <gtest/gtest.h>

TEST(LinkedListTest, InitiallyEmpty) {
    LinkedList<int> list;
    EXPECT_EQ(list.size(), 0);
}

TEST(LinkedListTest, AddElementsToFront) {
    LinkedList<int> list;
    list.add(10, 0);
    list.add(20, 0);
    list.add(30, 0);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 30);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 10);
}

TEST(LinkedListTest, AddElementsToEnd) {
    LinkedList<int> list;
    list.add(10, 0);
    list.add(20, 1);
    list.add(30, 2);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}

TEST(LinkedListTest, AddElementsToMiddle) {
    LinkedList<int> list;
    list.add(10, 0);  // [10]
    list.add(30, 1);  // [10, 30]
    list.add(20, 1);  // [10, 20, 30]

    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}

TEST(LinkedListTest, RemoveElements) {
    LinkedList<int> list;
    list.add(1, 0);
    list.add(2, 1);
    list.add(3, 2);  // [1, 2, 3]

    int removed = list.remove(1);  // Remove 2
    EXPECT_EQ(removed, 2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 3);
}

TEST(LinkedListTest, RemoveFromFrontAndEnd) {
    LinkedList<int> list;
    list.add(1, 0);
    list.add(2, 1);
    list.add(3, 2);

    EXPECT_EQ(list.remove(0), 1); // front
    EXPECT_EQ(list.remove(1), 3); // end
    EXPECT_EQ(list.get(0), 2);
    EXPECT_EQ(list.size(), 1);
}

TEST(LinkedListTest, GetInvalidIndexThrows) {
    LinkedList<int> list;
    list.add(1, 0);

    EXPECT_THROW(list.get(-1), std::out_of_range);
    EXPECT_THROW(list.get(2), std::out_of_range);
}

TEST(LinkedListTest, AddInvalidIndexThrows) {
    LinkedList<int> list;
    EXPECT_THROW(list.add(1, -1), std::out_of_range);
    EXPECT_THROW(list.add(1, 2), std::out_of_range);
}

TEST(LinkedListTest, RemoveInvalidIndexThrows) {
    LinkedList<int> list;
    EXPECT_THROW(list.remove(0), std::out_of_range);
    list.add(42, 0);
    EXPECT_THROW(list.remove(1), std::out_of_range);
}
