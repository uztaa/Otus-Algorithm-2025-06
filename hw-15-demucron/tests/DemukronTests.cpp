#include <gtest/gtest.h>
#include "DemukronAlgorithm.hpp"

/**
 * @brief Тест: Пустой граф (0 вершин)
 */
TEST(DemukronTest, EmptyGraph) {
    SpaceArray<int> adjList;
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 0);
    
    EXPECT_FALSE(result.hasCycle);
    EXPECT_EQ(result.levels.blockCount(), 0);
}

/**
 * @brief Тест: Одна вершина без рёбер
 */
TEST(DemukronTest, SingleVertex) {
    SpaceArray<int> adjList;
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 1);
    
    EXPECT_FALSE(result.hasCycle);
    EXPECT_EQ(result.levels.blockCount(), 1);
    EXPECT_EQ(result.levels.blockSize(0), 1);
    EXPECT_EQ(result.levels.get(0, 0), 0);
}

/**
 * @brief Тест: Линейный граф 0 → 1 → 2 → 3
 * Ожидаемый результат: каждая вершина на своём уровне
 */
TEST(DemukronTest, LinearGraph) {
    SpaceArray<int> adjList;
    
    adjList.add(0, 1);
    adjList.add(1, 2);
    adjList.add(2, 3);
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 4);
    
    EXPECT_FALSE(result.hasCycle);
    EXPECT_EQ(result.levels.blockCount(), 4);
    
    // Уровень 0: вершина 0
    EXPECT_EQ(result.levels.blockSize(0), 1);
    EXPECT_EQ(result.levels.get(0, 0), 0);
    
    // Уровень 1: вершина 1
    EXPECT_EQ(result.levels.blockSize(1), 1);
    EXPECT_EQ(result.levels.get(1, 0), 1);
    
    // Уровень 2: вершина 2
    EXPECT_EQ(result.levels.blockSize(2), 1);
    EXPECT_EQ(result.levels.get(2, 0), 2);
    
    // Уровень 3: вершина 3
    EXPECT_EQ(result.levels.blockSize(3), 1);
    EXPECT_EQ(result.levels.get(3, 0), 3);
}

/**
 * @brief Тест: Параллельные пути
 * 
 *     0 ──→ 2
 *     ↓     ↓
 *     1 ──→ 3
 * 
 * Ожидаемый результат:
 *   Уровень 0: [0]
 *   Уровень 1: [1, 2]
 *   Уровень 2: [3]
 */
TEST(DemukronTest, ParallelPaths) {
    SpaceArray<int> adjList;
    
    adjList.add(0, 1);
    adjList.add(0, 2);
    adjList.add(1, 3);
    adjList.add(2, 3);
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 4);
    
    EXPECT_FALSE(result.hasCycle);
    EXPECT_EQ(result.levels.blockCount(), 3);
    
    // Уровень 0: только вершина 0
    EXPECT_EQ(result.levels.blockSize(0), 1);
    EXPECT_EQ(result.levels.get(0, 0), 0);
    
    // Уровень 1: вершины 1 и 2
    EXPECT_EQ(result.levels.blockSize(1), 2);
    
    // Уровень 2: только вершина 3
    EXPECT_EQ(result.levels.blockSize(2), 1);
    EXPECT_EQ(result.levels.get(2, 0), 3);
}

/**
 * @brief Тест: Все вершины изолированы
 * Ожидаемый результат: все на уровне 0
 */
TEST(DemukronTest, IsolatedVertices) {
    SpaceArray<int> adjList;
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 5);
    
    EXPECT_FALSE(result.hasCycle);
    EXPECT_EQ(result.levels.blockCount(), 1);
    EXPECT_EQ(result.levels.blockSize(0), 5);
}

/**
 * @brief Тест: Простой цикл 0 → 1 → 0
 */
TEST(DemukronTest, SimpleCycle) {
    SpaceArray<int> adjList;
    
    adjList.add(0, 1);
    adjList.add(1, 0);
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 2);
    
    EXPECT_TRUE(result.hasCycle);
}

/**
 * @brief Тест: Самопетля 0 → 0
 */
TEST(DemukronTest, SelfLoop) {
    SpaceArray<int> adjList;
    
    adjList.add(0, 0);
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 1);
    
    EXPECT_TRUE(result.hasCycle);
}

/**
 * @brief Тест: Цикл в части графа
 * 
 *     0 ──→ 1 ──→ 2
 *           ↑     ↓
 *           └─────┘
 * 
 * Вершина 0 обработается, но 1-2 образуют цикл
 */
TEST(DemukronTest, PartialCycle) {
    SpaceArray<int> adjList;
    
    adjList.add(0, 1);
    adjList.add(1, 2);
    adjList.add(2, 1);
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 3);
    
    EXPECT_TRUE(result.hasCycle);
}

/**
 * @brief Тест: Несколько независимых компонент
 * 
 * Компонента 1: 0 → 1
 * Компонента 2: 2 → 3
 * 
 * Ожидаемый результат:
 *   Уровень 0: [0, 2]
 *   Уровень 1: [1, 3]
 */
TEST(DemukronTest, MultipleComponents) {
    SpaceArray<int> adjList;
    
    adjList.add(0, 1);
    adjList.add(2, 3);
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 4);
    
    EXPECT_FALSE(result.hasCycle);
    EXPECT_EQ(result.levels.blockCount(), 2);
    EXPECT_EQ(result.levels.blockSize(0), 2);
    EXPECT_EQ(result.levels.blockSize(1), 2);
}

/**
 * @brief Тест: Ромбовидный граф (diamond)
 * 
 *       0
 *      / \
 *     1   2
 *      \ /
 *       3
 * 
 * Ожидаемый результат:
 *   Уровень 0: [0]
 *   Уровень 1: [1, 2]
 *   Уровень 2: [3]
 */
TEST(DemukronTest, DiamondGraph) {
    SpaceArray<int> adjList;
    
    adjList.add(0, 1);
    adjList.add(0, 2);
    adjList.add(1, 3);
    adjList.add(2, 3);
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 4);
    
    EXPECT_FALSE(result.hasCycle);
    EXPECT_EQ(result.levels.blockCount(), 3);
    
    EXPECT_EQ(result.levels.blockSize(0), 1);
    EXPECT_EQ(result.levels.get(0, 0), 0);
    
    EXPECT_EQ(result.levels.blockSize(1), 2);
    
    EXPECT_EQ(result.levels.blockSize(2), 1);
    EXPECT_EQ(result.levels.get(2, 0), 3);
}

/**
 * @brief Тест: Граф из лекции (пример с 14 вершинами)
 * Проверяем, что все вершины распределены по уровням корректно
 */
TEST(DemukronTest, LectureExample) {
    SpaceArray<int> adjList;
    
    // Рёбра из лекции (нумерация с 0):
    // Строим по матрице смежности
    adjList.add(0, 3);   // 1 → 4
    adjList.add(1, 12);  // 2 → 13
    adjList.add(3, 2);   // 4 → 3
    adjList.add(4, 2);   // 5 → 3
    adjList.add(4, 6);   // 5 → 7
    adjList.add(4, 8);   // 5 → 9
    adjList.add(5, 10);  // 6 → 11
    adjList.add(5, 11);  // 6 → 12
    adjList.add(5, 12);  // 6 → 13
    adjList.add(6, 10);  // 7 → 11
    adjList.add(7, 1);   // 8 → 2
    adjList.add(7, 3);   // 8 → 4
    adjList.add(7, 5);   // 8 → 6
    adjList.add(7, 6);   // 8 → 7
    adjList.add(8, 0);   // 9 → 1
    adjList.add(9, 0);   // 10 → 1
    adjList.add(9, 12);  // 10 → 13
    adjList.add(10, 2);  // 11 → 3
    adjList.add(12, 3);  // 13 → 4
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 14);
    
    EXPECT_FALSE(result.hasCycle);
    
    // Проверяем, что все 14 вершин распределены
    int totalVertices = 0;
    for (int level = 0; level < result.levels.blockCount(); ++level) {
        totalVertices += result.levels.blockSize(level);
    }
    EXPECT_EQ(totalVertices, 14);
}

/**
 * @brief Тест: Проверка корректности уровней
 * Для каждой вершины все её предшественники должны быть на более ранних уровнях
 */
TEST(DemukronTest, LevelOrderCorrectness) {
    SpaceArray<int> adjList;
    
    // Строим граф: 0 → 1 → 3, 0 → 2 → 3
    adjList.add(0, 1);
    adjList.add(0, 2);
    adjList.add(1, 3);
    adjList.add(2, 3);
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 4);
    
    EXPECT_FALSE(result.hasCycle);
    
    // Создаём обратное отображение: вершина → уровень
    FactorArray<int> vertexLevel;
    for (int i = 0; i < 4; ++i) {
        vertexLevel.add(-1, i);
    }
    
    for (int level = 0; level < result.levels.blockCount(); ++level) {
        for (int i = 0; i < result.levels.blockSize(level); ++i) {
            int v = result.levels.get(level, i);
            // Обновляем уровень вершины
            vertexLevel.remove(v);
            vertexLevel.add(level, v);
        }
    }
    
    // Проверяем: для каждого ребра u → v должно быть level[u] < level[v]
    // 0 → 1: level[0] < level[1]
    EXPECT_LT(vertexLevel.get(0), vertexLevel.get(1));
    // 0 → 2: level[0] < level[2]
    EXPECT_LT(vertexLevel.get(0), vertexLevel.get(2));
    // 1 → 3: level[1] < level[3]
    EXPECT_LT(vertexLevel.get(1), vertexLevel.get(3));
    // 2 → 3: level[2] < level[3]
    EXPECT_LT(vertexLevel.get(2), vertexLevel.get(3));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}