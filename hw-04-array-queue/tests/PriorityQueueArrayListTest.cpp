#include <gtest/gtest.h>
#include "PriorityQueueArrayList.h"

TEST(PriorityQueueArrayListTest, EnqueueDequeueBasic) {
    PriorityQueueArrayList<int> queue(1, 3);
    queue.enqueue(2, 42);
    queue.enqueue(1, 99);
    queue.enqueue(3, 7);
    queue.enqueue(1, 100);

    EXPECT_EQ(queue.dequeue(), 99);   // priority 1, first inserted
    EXPECT_EQ(queue.dequeue(), 100);  // priority 1, second inserted
    EXPECT_EQ(queue.dequeue(), 42);   // priority 2
    EXPECT_EQ(queue.dequeue(), 7);    // priority 3
    EXPECT_TRUE(queue.empty());
}

TEST(PriorityQueueArrayListTest, EnqueueMultiplePriorities) {
    PriorityQueueArrayList<std::string> queue(0, 2);
    queue.enqueue(2, "low");
    queue.enqueue(0, "high");
    queue.enqueue(1, "medium");

    EXPECT_EQ(queue.dequeue(), "high");
    EXPECT_EQ(queue.dequeue(), "medium");
    EXPECT_EQ(queue.dequeue(), "low");
    EXPECT_TRUE(queue.empty());
}

TEST(PriorityQueueArrayListTest, DequeueFromEmptyThrows) {
    PriorityQueueArrayList<int> queue(0, 1);
    EXPECT_THROW(queue.dequeue(), std::out_of_range);
}

TEST(PriorityQueueArrayListTest, EnqueuePriorityOutOfRangeThrows) {
    PriorityQueueArrayList<int> queue(5, 7);
    EXPECT_THROW(queue.enqueue(4, 1), std::out_of_range);
    EXPECT_THROW(queue.enqueue(8, 2), std::out_of_range);
}

TEST(PriorityQueueArrayListTest, EmptyReturnsTrueWhenNoElements) {
    PriorityQueueArrayList<int> queue(0, 2);
    EXPECT_TRUE(queue.empty());
    queue.enqueue(1, 10);
    EXPECT_FALSE(queue.empty());
    queue.dequeue();
    EXPECT_TRUE(queue.empty());
}
