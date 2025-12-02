#include <gtest/gtest.h>
#include "ChainingHashTable.hpp"
#include "HashFunction.hpp"

// ========== БАЗОВЫЕ ТЕСТЫ ==========

TEST(ChainingHashTableTest, InsertAndSearch) {
    // Вставляем и ищем элементы
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    table.insert(42, 100);
    int value = 0;
    EXPECT_TRUE(table.search(42, value));
    EXPECT_EQ(value, 100);
}

TEST(ChainingHashTableTest, UpdateValue) {
    // Обновляем значение существующего ключа
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    table.insert(5, 50);
    table.insert(5, 500);
    
    int value = 0;
    EXPECT_TRUE(table.search(5, value));
    EXPECT_EQ(value, 500);
    EXPECT_EQ(table.size(), 1);
}

TEST(ChainingHashTableTest, Remove) {
    // Удаляем элемент
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    table.insert(42, 100);
    EXPECT_TRUE(table.remove(42));
    
    int value = 0;
    EXPECT_FALSE(table.search(42, value));
    EXPECT_EQ(table.size(), 0);
}

TEST(ChainingHashTableTest, RemoveNonExistent) {
    // Удаляем несуществующий элемент
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    EXPECT_FALSE(table.remove(999));
}

TEST(ChainingHashTableTest, SearchNonExistent) {
    // Ищем несуществующий элемент
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    table.insert(1, 10);
    int value = 0;
    EXPECT_FALSE(table.search(999, value));
}

// ========== ТЕСТЫ РАЗМЕРА ==========

TEST(ChainingHashTableTest, Size) {
    // Проверяем размер таблицы
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
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

TEST(ChainingHashTableTest, Clear) {
    // Очищаем таблицу
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    for (int i = 0; i < 10; ++i) {
        table.insert(i, i * 10);
    }
    
    EXPECT_EQ(table.size(), 10);
    table.clear();
    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());
}

// ========== ТЕСТЫ LOAD FACTOR ==========

TEST(ChainingHashTableTest, LoadFactor) {
    // Проверяем load factor
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    EXPECT_DOUBLE_EQ(table.loadFactor(), 0.0);
    
    table.insert(1, 10);
    EXPECT_DOUBLE_EQ(table.loadFactor(), 1.0 / 11.0);
    
    table.insert(2, 20);
    EXPECT_DOUBLE_EQ(table.loadFactor(), 2.0 / 11.0);
}

// ========== ТЕСТЫ КОЛЛИЗИЙ ==========

TEST(ChainingHashTableTest, CollisionHandling) {
    // Обработка коллизий (модульная хеш-функция)
    ChainingHashTable<SimpleHashFunction<>> table(7);
    
    // 7 % 7 = 0, 14 % 7 = 0, 21 % 7 = 0 (все в одну цепь)
    table.insert(7, 70);
    table.insert(14, 140);
    table.insert(21, 210);
    
    EXPECT_EQ(table.size(), 3);
    
    int value = 0;
    EXPECT_TRUE(table.search(7, value));
    EXPECT_EQ(value, 70);
    
    EXPECT_TRUE(table.search(14, value));
    EXPECT_EQ(value, 140);
    
    EXPECT_TRUE(table.search(21, value));
    EXPECT_EQ(value, 210);
}

// ========== ТЕСТЫ REHASHING ==========

TEST(ChainingHashTableTest, AutomaticRehashing) {
    // Автоматическое расширение таблицы
    ChainingHashTable<SimpleHashFunction<>> table(5);
    
    int initial_capacity = table.getCapacity();
    
    // Вставляем элементы до превышения load factor
    for (int i = 0; i < 10; ++i) {
        table.insert(i, i * 10);
    }
    
    int final_capacity = table.getCapacity();
    
    // Таблица должна была расшириться
    EXPECT_GT(final_capacity, initial_capacity);
    
    // Все элементы должны быть на месте
    for (int i = 0; i < 10; ++i) {
        int value = 0;
        EXPECT_TRUE(table.search(i, value));
        EXPECT_EQ(value, i * 10);
    }
}

// ========== ТЕСТЫ МНОЖЕСТВЕННЫХ ОПЕРАЦИЙ ==========

TEST(ChainingHashTableTest, MultipleOperations) {
    // Комбинированный тест операций
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    // Вставляем 5 элементов
    for (int i = 1; i <= 5; ++i) {
        table.insert(i, i * 100);
    }
    EXPECT_EQ(table.size(), 5);
    
    // Обновляем 2 элемента
    table.insert(2, 999);
    table.insert(4, 888);
    EXPECT_EQ(table.size(), 5);
    
    // Проверяем обновления
    int value = 0;
    table.search(2, value);
    EXPECT_EQ(value, 999);
    
    table.search(4, value);
    EXPECT_EQ(value, 888);
    
    // Удаляем 2 элемента
    table.remove(1);
    table.remove(5);
    EXPECT_EQ(table.size(), 3);
    
    // Проверяем оставшиеся
    EXPECT_TRUE(table.search(2, value));
    EXPECT_TRUE(table.search(3, value));
    EXPECT_TRUE(table.search(4, value));
    
    EXPECT_FALSE(table.search(1, value));
    EXPECT_FALSE(table.search(5, value));
}

// ========== ТЕСТЫ РАЗНЫХ ХЕШ-ФУНКЦИЙ ==========

TEST(ChainingHashTableTest, DifferentHashFunctions) {
    // Проверяем работу с разными хеш-функциями
    ChainingHashTable<MultiplicativeHashFunction<>> table1(11);
    ChainingHashTable<BitwiseHashFunction<>> table2(11);
    
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
