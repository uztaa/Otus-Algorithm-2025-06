#include <gtest/gtest.h>
#include "OpenAddressingHashTable.hpp"
#include "HashFunction.hpp"

// ========== БАЗОВЫЕ ТЕСТЫ ==========

TEST(OpenAddressingHashTableTest, InsertAndSearch) {
    OpenAddressingHashTable<> table(11, 0.7);
    
    table.insert(42, 100);
    int value = 0;
    EXPECT_TRUE(table.search(42, value));
    EXPECT_EQ(value, 100);
}

TEST(OpenAddressingHashTableTest, UpdateValue) {
    OpenAddressingHashTable<> table(11, 0.7);
    
    table.insert(5, 50);
    table.insert(5, 500);
    
    int value = 0;
    EXPECT_TRUE(table.search(5, value));
    EXPECT_EQ(value, 500);
    EXPECT_EQ(table.size(), 1);
}

TEST(OpenAddressingHashTableTest, Remove) {
    OpenAddressingHashTable<> table(11, 0.7);
    
    table.insert(42, 100);
    EXPECT_TRUE(table.remove(42));
    
    int value = 0;
    EXPECT_FALSE(table.search(42, value));
    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.getDeletedCount(), 1);
}

TEST(OpenAddressingHashTableTest, RemoveNonExistent) {
    OpenAddressingHashTable<> table(11, 0.7);
    
    EXPECT_FALSE(table.remove(999));
}

TEST(OpenAddressingHashTableTest, SearchNonExistent) {
    OpenAddressingHashTable<> table(11, 0.7);
    
    table.insert(1, 10);
    int value = 0;
    EXPECT_FALSE(table.search(999, value));
}

// ========== ТЕСТЫ ЛЕНИВОГО УДАЛЕНИЯ ==========

TEST(OpenAddressingHashTableTest, LazyDeletion) {
    OpenAddressingHashTable<> table(11, 0.7);
    
    table.insert(1, 10);
    table.insert(2, 20);
    table.insert(3, 30);
    
    EXPECT_EQ(table.size(), 3);
    EXPECT_EQ(table.getDeletedCount(), 0);
    
    table.remove(2);
    
    EXPECT_EQ(table.size(), 2);
    EXPECT_EQ(table.getDeletedCount(), 1);
    
    int value = 0;
    EXPECT_TRUE(table.search(1, value));
    EXPECT_TRUE(table.search(3, value));
    EXPECT_FALSE(table.search(2, value));
}

// ========== ТЕСТЫ РАЗМЕРА ==========

TEST(OpenAddressingHashTableTest, Size) {
    OpenAddressingHashTable<> table(11, 0.7);
    
    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());
    
    table.insert(1, 10);
    EXPECT_EQ(table.size(), 1);
    EXPECT_FALSE(table.empty());
    
    table.insert(2, 20);
    EXPECT_EQ(table.size(), 2);
    
    table.remove(1);
    EXPECT_EQ(table.size(), 1);
}

TEST(OpenAddressingHashTableTest, Clear) {
    OpenAddressingHashTable<> table(11, 0.7);
    
    for (int i = 0; i < 10; ++i) {
        table.insert(i, i * 10);
    }
    
    EXPECT_EQ(table.size(), 10);
    table.clear();
    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.getDeletedCount(), 0);
}

// ========== ТЕСТЫ LOAD FACTOR ==========

TEST(OpenAddressingHashTableTest, LoadFactor) {
    OpenAddressingHashTable<> table(11, 0.7);
    
    EXPECT_DOUBLE_EQ(table.loadFactor(), 0.0);
    
    table.insert(1, 10);
    EXPECT_DOUBLE_EQ(table.loadFactor(), 1.0 / 11.0);
    
    table.insert(2, 20);
    EXPECT_DOUBLE_EQ(table.loadFactor(), 2.0 / 11.0);
}

// ========== ТЕСТЫ REHASHING ==========

TEST(OpenAddressingHashTableTest, RehashingOnLoadFactor) {
    OpenAddressingHashTable<> table(5, 0.7);
    
    int initial_capacity = table.getCapacity();
    
    for (int i = 0; i < 10; ++i) {
        table.insert(i, i * 10);
    }
    
    int final_capacity = table.getCapacity();
    
    EXPECT_GT(final_capacity, initial_capacity);
    
    for (int i = 0; i < 10; ++i) {
        int value = 0;
        EXPECT_TRUE(table.search(i, value));
        EXPECT_EQ(value, i * 10);
    }
}

TEST(OpenAddressingHashTableTest, RehashingOnDeletedCells) {
    // Используем большую таблицу чтобы избежать rehash при вставке/удалении
    OpenAddressingHashTable<> table(101, 0.7);
    
    // Заполняем таблицу несколькими элементами
    for (int i = 0; i < 3; ++i) {
        table.insert(i, i * 10);
    }
    
    EXPECT_EQ(table.size(), 3);
    EXPECT_EQ(table.getDeletedCount(), 0);
    
    // Удаляем элементы - deleted должны увеличиваться
    table.remove(0);
    EXPECT_EQ(table.size(), 2);
    EXPECT_EQ(table.getDeletedCount(), 1);
    
    table.remove(1);
    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(table.getDeletedCount(), 2);
    
    table.remove(2);
    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.getDeletedCount(), 3);
    
    // Проверяем что все удалены
    int value = 0;
    EXPECT_FALSE(table.search(0, value));
    EXPECT_FALSE(table.search(1, value));
    EXPECT_FALSE(table.search(2, value));
    
    // Вставляем новый элемент
    table.insert(100, 1000);
    EXPECT_EQ(table.size(), 1);
    EXPECT_TRUE(table.search(100, value));
    EXPECT_EQ(value, 1000);
}

// ========== ТЕСТЫ КВАДРАТИЧНОГО ПРОБИРОВАНИЯ ==========

TEST(OpenAddressingHashTableTest, QuadraticProbing) {
    OpenAddressingHashTable<SimpleHashFunction<>, SecondaryHashFunction<>> table(7, 0.7);
    
    table.insert(0, 10);
    table.insert(7, 70);
    table.insert(14, 140);
    
    EXPECT_EQ(table.size(), 3);
    
    int value = 0;
    EXPECT_TRUE(table.search(0, value));
    EXPECT_EQ(value, 10);
    
    EXPECT_TRUE(table.search(7, value));
    EXPECT_EQ(value, 70);
    
    EXPECT_TRUE(table.search(14, value));
    EXPECT_EQ(value, 140);
}

// ========== ТЕСТЫ МНОЖЕСТВЕННЫХ ОПЕРАЦИЙ ==========

TEST(OpenAddressingHashTableTest, MultipleOperations) {
    OpenAddressingHashTable<> table(11, 0.7);
    
    for (int i = 1; i <= 5; ++i) {
        table.insert(i, i * 100);
    }
    EXPECT_EQ(table.size(), 5);
    
    table.insert(2, 999);
    table.insert(4, 888);
    EXPECT_EQ(table.size(), 5);
    
    int value = 0;
    table.search(2, value);
    EXPECT_EQ(value, 999);
    
    table.remove(1);
    table.remove(5);
    EXPECT_EQ(table.size(), 3);
    
    EXPECT_TRUE(table.search(2, value));
    EXPECT_TRUE(table.search(3, value));
    EXPECT_TRUE(table.search(4, value));
    
    EXPECT_FALSE(table.search(1, value));
    EXPECT_FALSE(table.search(5, value));
}

// ========== ТЕСТЫ РАЗНЫХ ХЕШ-ФУНКЦИЙ ==========

TEST(OpenAddressingHashTableTest, DifferentHashFunctions) {
    OpenAddressingHashTable<MultiplicativeHashFunction<>, SecondaryHashFunction<>> table1(11, 0.7);
    OpenAddressingHashTable<BitwiseHashFunction<>, SecondaryHashFunction<>> table2(11, 0.7);
    
    for (int i = 0; i < 10; ++i) {
        table1.insert(i, i * 10);
        table2.insert(i, i * 10);
    }
    
    EXPECT_EQ(table1.size(), 10);
    EXPECT_EQ(table2.size(), 10);
    
    int value = 0;
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(table1.search(i, value));
        EXPECT_EQ(value, i * 10);
        
        EXPECT_TRUE(table2.search(i, value));
        EXPECT_EQ(value, i * 10);
    }
}
