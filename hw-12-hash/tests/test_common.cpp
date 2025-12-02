#include <gtest/gtest.h>
#include "Common.hpp"

// ========== ТЕСТЫ PRIME UTILS ==========

TEST(PrimeUtilsTest, IsPrime) {
    EXPECT_FALSE(PrimeUtils::isPrime(0));
    EXPECT_FALSE(PrimeUtils::isPrime(1));
    EXPECT_TRUE(PrimeUtils::isPrime(2));
    EXPECT_TRUE(PrimeUtils::isPrime(3));
    EXPECT_FALSE(PrimeUtils::isPrime(4));
    EXPECT_TRUE(PrimeUtils::isPrime(5));
    EXPECT_TRUE(PrimeUtils::isPrime(7));
    EXPECT_TRUE(PrimeUtils::isPrime(11));
    EXPECT_TRUE(PrimeUtils::isPrime(13));
    EXPECT_FALSE(PrimeUtils::isPrime(15));
}

TEST(PrimeUtilsTest, GetNextPrime) {
    EXPECT_EQ(PrimeUtils::getNextPrime(1), 2);
    EXPECT_EQ(PrimeUtils::getNextPrime(2), 2);
    EXPECT_EQ(PrimeUtils::getNextPrime(3), 3);
    EXPECT_EQ(PrimeUtils::getNextPrime(4), 5);
    EXPECT_EQ(PrimeUtils::getNextPrime(10), 11);
    EXPECT_EQ(PrimeUtils::getNextPrime(11), 11);
}

TEST(PrimeUtilsTest, GetNextPrimeGreater) {
    EXPECT_EQ(PrimeUtils::getNextPrimeGreater(1), 2);
    EXPECT_EQ(PrimeUtils::getNextPrimeGreater(2), 3);
    EXPECT_EQ(PrimeUtils::getNextPrimeGreater(10), 11);
    EXPECT_EQ(PrimeUtils::getNextPrimeGreater(11), 13);
}

TEST(PrimeUtilsTest, GetNextSizeForRehash) {
    int size = PrimeUtils::getNextSizeForRehash(11);
    EXPECT_TRUE(PrimeUtils::isPrime(size));
    EXPECT_GT(size, 11);
    EXPECT_LT(size, 11 * 3);
}

// ========== ТЕСТЫ СТРУКТУР ==========

TEST(CellTest, DefaultState) {
    Cell cell;
    EXPECT_EQ(cell.key, -1);
    EXPECT_EQ(cell.value, -1);
    EXPECT_EQ(cell.state, CellState::EMPTY);
}

TEST(CellTest, ActiveState) {
    Cell cell;
    cell.key = 42;
    cell.value = 100;
    cell.state = CellState::ACTIVE;
    
    EXPECT_EQ(cell.key, 42);
    EXPECT_EQ(cell.value, 100);
    EXPECT_EQ(cell.state, CellState::ACTIVE);
}

TEST(ListNodeTest, Construction) {
    ListNode node(42, 100);
    EXPECT_EQ(node.key, 42);
    EXPECT_EQ(node.value, 100);
    EXPECT_EQ(node.next, nullptr);
}

TEST(ListNodeTest, Linking) {
    ListNode node1(10, 100);
    ListNode node2(20, 200);
    
    node1.next = &node2;
    EXPECT_EQ(node1.next->key, 20);
    EXPECT_EQ(node1.next->value, 200);
}

// ========== ТЕСТЫ КОНФИГУРАЦИИ ==========

TEST(HashTableConfigTest, Constants) {
    EXPECT_DOUBLE_EQ(HashTableConfig::LOAD_FACTOR_THRESHOLD, 0.75);
    EXPECT_DOUBLE_EQ(HashTableConfig::OPEN_ADDR_LOAD_FACTOR, 0.7);
    EXPECT_DOUBLE_EQ(HashTableConfig::DELETE_RATIO_THRESHOLD, 0.25);
    EXPECT_EQ(HashTableConfig::MAX_PROBES, 100);
    EXPECT_EQ(HashTableConfig::INITIAL_CAPACITY, 11);
}

// ========== ТЕСТЫ СТАТИСТИКИ ==========

TEST(HashTableStatsTest, DefaultStats) {
    HashTableStats stats;
    EXPECT_EQ(stats.total_elements, 0);
    EXPECT_EQ(stats.table_capacity, 0);
    EXPECT_EQ(stats.deleted_elements, 0);
    EXPECT_EQ(stats.collisions, 0);
    EXPECT_DOUBLE_EQ(stats.load_factor, 0.0);
}
