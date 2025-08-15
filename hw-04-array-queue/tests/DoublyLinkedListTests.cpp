#include "DoublyLinkedList.h"
#include <gtest/gtest.h>

TEST(DoublyLinkedListTest, InitiallyEmpty) {
    DoublyLinkedList<int> list;
    EXPECT_EQ(list.size(), 0);
}

TEST(DoublyLinkedListTest, AddElementsToFront) {
    DoublyLinkedList<int> list;
    list.add(10, 0);
    list.add(20, 0);
    list.add(30, 0);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 30);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 10);
}

TEST(DoublyLinkedListTest, AddElementsToEnd) {
    DoublyLinkedList<int> list;
    list.add(10, 0);
    list.add(20, 1);
    list.add(30, 2);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}

TEST(DoublyLinkedListTest, AddElementsToMiddle) {
    DoublyLinkedList<int> list;
    list.add(10, 0);  // [10]
    list.add(30, 1);  // [10, 30]
    list.add(20, 1);  // [10, 20, 30]

    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}

TEST(DoublyLinkedListTest, RemoveElements) {
    DoublyLinkedList<int> list;
    list.add(1, 0);
    list.add(2, 1);
    list.add(3, 2);  // [1, 2, 3]

    int removed = list.remove(1);  // Remove 2
    EXPECT_EQ(removed, 2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 3);
}

TEST(DoublyLinkedListTest, RemoveFromFrontAndEnd) {
    DoublyLinkedList<int> list;
    list.add(1, 0);
    list.add(2, 1);
    list.add(3, 2);

    EXPECT_EQ(list.remove(0), 1); // front
    EXPECT_EQ(list.remove(1), 3); // end
    EXPECT_EQ(list.get(0), 2);
    EXPECT_EQ(list.size(), 1);
}

TEST(DoublyLinkedListTest, GetInvalidIndexThrows) {
    DoublyLinkedList<int> list;
    list.add(1, 0);

    EXPECT_THROW(list.get(-1), std::out_of_range);
    EXPECT_THROW(list.get(2), std::out_of_range);
}

TEST(DoublyLinkedListTest, AddInvalidIndexThrows) {
    DoublyLinkedList<int> list;
    EXPECT_THROW(list.add(1, -1), std::out_of_range);
    EXPECT_THROW(list.add(1, 2), std::out_of_range);
}

TEST(DoublyLinkedListTest, RemoveInvalidIndexThrows) {
    DoublyLinkedList<int> list;
    EXPECT_THROW(list.remove(0), std::out_of_range);
    list.add(42, 0);
    EXPECT_THROW(list.remove(1), std::out_of_range);
}