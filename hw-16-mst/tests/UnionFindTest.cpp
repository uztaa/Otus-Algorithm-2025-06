#include <gtest/gtest.h>
#include "UnionFind.hpp"

// ============================================
// Базовые тесты
// ============================================

TEST(UnionFindTest, InitialState) {
    UnionFind uf(5);
    
    // Изначально 5 компонент
    EXPECT_EQ(uf.componentCount(), 5);
    EXPECT_EQ(uf.size(), 5);
    
    // Каждый элемент в своём множестве
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(uf.find(i), i);
    }
}

TEST(UnionFindTest, SingleElement) {
    UnionFind uf(1);
    
    EXPECT_EQ(uf.componentCount(), 1);
    EXPECT_EQ(uf.find(0), 0);
}

TEST(UnionFindTest, EmptySet) {
    UnionFind uf(0);
    
    EXPECT_EQ(uf.componentCount(), 0);
    EXPECT_EQ(uf.size(), 0);
}

// ============================================
// Тесты операции merge
// ============================================

TEST(UnionFindTest, MergeTwoElements) {
    UnionFind uf(3);
    
    // Объединяем 0 и 1
    EXPECT_TRUE(uf.merge(0, 1));
    EXPECT_EQ(uf.componentCount(), 2);
    
    // Теперь они в одном множестве
    EXPECT_EQ(uf.find(0), uf.find(1));
    
    // Элемент 2 всё ещё отдельно
    EXPECT_NE(uf.find(0), uf.find(2));
}

TEST(UnionFindTest, MergeSameElement) {
    UnionFind uf(3);
    
    // Объединение элемента с самим собой
    EXPECT_FALSE(uf.merge(1, 1));
    EXPECT_EQ(uf.componentCount(), 3);
}

TEST(UnionFindTest, MergeAlreadyConnected) {
    UnionFind uf(3);
    
    uf.merge(0, 1);
    EXPECT_EQ(uf.componentCount(), 2);
    
    // Повторное объединение не должно изменять состояние
    EXPECT_FALSE(uf.merge(0, 1));
    EXPECT_FALSE(uf.merge(1, 0));
    EXPECT_EQ(uf.componentCount(), 2);
}

TEST(UnionFindTest, ChainMerge) {
    UnionFind uf(5);
    
    // Объединяем в цепочку: 0-1, 1-2, 2-3, 3-4
    EXPECT_TRUE(uf.merge(0, 1));
    EXPECT_TRUE(uf.merge(1, 2));
    EXPECT_TRUE(uf.merge(2, 3));
    EXPECT_TRUE(uf.merge(3, 4));
    
    EXPECT_EQ(uf.componentCount(), 1);
    
    // Все элементы в одном множестве
    int root = uf.find(0);
    for (int i = 1; i < 5; ++i) {
        EXPECT_EQ(uf.find(i), root);
    }
}

TEST(UnionFindTest, TwoComponents) {
    UnionFind uf(6);
    
    // Компонента 1: {0, 1, 2}
    uf.merge(0, 1);
    uf.merge(1, 2);
    
    // Компонента 2: {3, 4, 5}
    uf.merge(3, 4);
    uf.merge(4, 5);
    
    EXPECT_EQ(uf.componentCount(), 2);
    
    // Проверяем связность внутри компонент
    EXPECT_TRUE(uf.connected(0, 1));
    EXPECT_TRUE(uf.connected(0, 2));
    EXPECT_TRUE(uf.connected(1, 2));
    
    EXPECT_TRUE(uf.connected(3, 4));
    EXPECT_TRUE(uf.connected(3, 5));
    EXPECT_TRUE(uf.connected(4, 5));
    
    // Проверяем несвязность между компонентами
    EXPECT_FALSE(uf.connected(0, 3));
    EXPECT_FALSE(uf.connected(1, 4));
    EXPECT_FALSE(uf.connected(2, 5));
}

TEST(UnionFindTest, MergeComponents) {
    UnionFind uf(4);
    
    // Создаём две компоненты
    uf.merge(0, 1);  // {0, 1}
    uf.merge(2, 3);  // {2, 3}
    
    EXPECT_EQ(uf.componentCount(), 2);
    
    // Объединяем компоненты
    EXPECT_TRUE(uf.merge(1, 2));
    EXPECT_EQ(uf.componentCount(), 1);
    
    // Все связаны
    EXPECT_TRUE(uf.connected(0, 3));
}

// ============================================
// Тесты операции connected
// ============================================

TEST(UnionFindTest, ConnectedInitially) {
    UnionFind uf(3);
    
    // Изначально элементы не связаны
    EXPECT_FALSE(uf.connected(0, 1));
    EXPECT_FALSE(uf.connected(1, 2));
    EXPECT_FALSE(uf.connected(0, 2));
    
    // Каждый элемент связан сам с собой
    EXPECT_TRUE(uf.connected(0, 0));
    EXPECT_TRUE(uf.connected(1, 1));
    EXPECT_TRUE(uf.connected(2, 2));
}

TEST(UnionFindTest, ConnectedAfterMerge) {
    UnionFind uf(4);
    
    uf.merge(0, 1);
    uf.merge(2, 3);
    
    EXPECT_TRUE(uf.connected(0, 1));
    EXPECT_TRUE(uf.connected(2, 3));
    EXPECT_FALSE(uf.connected(0, 2));
    EXPECT_FALSE(uf.connected(1, 3));
}

// ============================================
// Тесты сжатия путей
// ============================================

TEST(UnionFindTest, PathCompression) {
    UnionFind uf(10);
    
    // Создаём длинную цепочку: 0-1-2-3-4-5-6-7-8-9
    for (int i = 0; i < 9; ++i) {
        uf.merge(i, i + 1);
    }
    
    EXPECT_EQ(uf.componentCount(), 1);
    
    // После find(9) путь должен сжаться
    int root1 = uf.find(9);
    int root2 = uf.find(0);
    
    // Все должны указывать на один корень
    EXPECT_EQ(root1, root2);
    
    // После сжатия повторные find должны быть быстрыми
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(uf.find(i), root1);
    }
}

// ============================================
// Тесты для больших множеств
// ============================================

TEST(UnionFindTest, LargeSet) {
    const int n = 1000;
    UnionFind uf(n);
    
    EXPECT_EQ(uf.componentCount(), n);
    
    // Объединяем все элементы
    for (int i = 0; i < n - 1; ++i) {
        EXPECT_TRUE(uf.merge(i, i + 1));
    }
    
    EXPECT_EQ(uf.componentCount(), 1);
    
    // Все связаны
    int root = uf.find(0);
    for (int i = 1; i < n; ++i) {
        EXPECT_EQ(uf.find(i), root);
    }
}

TEST(UnionFindTest, RandomMerges) {
    UnionFind uf(100);
    
    // Серия merge в разном порядке
    uf.merge(50, 60);
    uf.merge(10, 20);
    uf.merge(60, 70);
    uf.merge(20, 30);
    uf.merge(70, 10);  // Соединяем две цепочки
    
    // Теперь {10, 20, 30, 50, 60, 70} в одной компоненте
    EXPECT_TRUE(uf.connected(10, 50));
    EXPECT_TRUE(uf.connected(30, 70));
    EXPECT_TRUE(uf.connected(20, 60));
    
    // Остальные отдельно
    EXPECT_FALSE(uf.connected(0, 10));
    EXPECT_FALSE(uf.connected(99, 50));
}

// ============================================
// Тесты для использования в алгоритмах MST
// ============================================

TEST(UnionFindTest, MSTScenario) {
    // Симуляция работы алгоритма Краскала
    // Граф: 0-1 (w=1), 1-2 (w=2), 0-2 (w=3)
    // MST: 0-1, 1-2
    
    UnionFind uf(3);
    
    // Добавляем ребро 0-1
    EXPECT_TRUE(uf.merge(0, 1));
    EXPECT_EQ(uf.componentCount(), 2);
    
    // Добавляем ребро 1-2
    EXPECT_TRUE(uf.merge(1, 2));
    EXPECT_EQ(uf.componentCount(), 1);
    
    // Ребро 0-2 образует цикл — не добавляем
    EXPECT_FALSE(uf.merge(0, 2));
    EXPECT_EQ(uf.componentCount(), 1);
}