#include <gtest/gtest.h>
#include "FloydWarshall.hpp"
#include "GraphMatrix.hpp"

class FloydWarshallTest : public ::testing::Test {
protected:
    static constexpr int INF = AllPairsShortestPathResult::INF;
};

// ============================================
// Базовые тесты
// ============================================

TEST_F(FloydWarshallTest, EmptyGraph) {
    GraphMatrix g(0);
    auto result = FloydWarshall::execute(g);
    
    EXPECT_EQ(result.vertexCount, 0);
    EXPECT_FALSE(result.hasNegativeCycle);
}

TEST_F(FloydWarshallTest, SingleVertex) {
    GraphMatrix g(1);
    auto result = FloydWarshall::execute(g);
    
    EXPECT_EQ(result.vertexCount, 1);
    EXPECT_EQ(result.getDistance(0, 0), 0);
    EXPECT_FALSE(result.hasNegativeCycle);
}

TEST_F(FloydWarshallTest, TwoVerticesConnected) {
    GraphMatrix g(2);
    g.addUndirectedEdge(0, 1, 5);
    
    auto result = FloydWarshall::execute(g);
    
    EXPECT_EQ(result.getDistance(0, 0), 0);
    EXPECT_EQ(result.getDistance(0, 1), 5);
    EXPECT_EQ(result.getDistance(1, 0), 5);
    EXPECT_EQ(result.getDistance(1, 1), 0);
    EXPECT_FALSE(result.hasNegativeCycle);
}

TEST_F(FloydWarshallTest, TwoVerticesDisconnected) {
    GraphMatrix g(2);
    // Нет рёбер
    
    auto result = FloydWarshall::execute(g);
    
    EXPECT_EQ(result.getDistance(0, 0), 0);
    EXPECT_EQ(result.getDistance(0, 1), INF);
    EXPECT_EQ(result.getDistance(1, 0), INF);
    EXPECT_EQ(result.getDistance(1, 1), 0);
}

// ============================================
// Тесты на графах
// ============================================

TEST_F(FloydWarshallTest, Triangle) {
    GraphMatrix g(3);
    g.addUndirectedEdge(0, 1, 1);
    g.addUndirectedEdge(1, 2, 2);
    g.addUndirectedEdge(0, 2, 4);
    
    auto result = FloydWarshall::execute(g);
    
    // От 0
    EXPECT_EQ(result.getDistance(0, 0), 0);
    EXPECT_EQ(result.getDistance(0, 1), 1);
    EXPECT_EQ(result.getDistance(0, 2), 3);  // 0->1->2 = 1+2 = 3 < 4
    
    // От 1
    EXPECT_EQ(result.getDistance(1, 0), 1);
    EXPECT_EQ(result.getDistance(1, 1), 0);
    EXPECT_EQ(result.getDistance(1, 2), 2);
    
    // От 2
    EXPECT_EQ(result.getDistance(2, 0), 3);
    EXPECT_EQ(result.getDistance(2, 1), 2);
    EXPECT_EQ(result.getDistance(2, 2), 0);
}

TEST_F(FloydWarshallTest, LinearGraph) {
    GraphMatrix g(4);
    g.addUndirectedEdge(0, 1, 1);
    g.addUndirectedEdge(1, 2, 2);
    g.addUndirectedEdge(2, 3, 3);
    
    auto result = FloydWarshall::execute(g);
    
    EXPECT_EQ(result.getDistance(0, 3), 6);  // 0->1->2->3 = 1+2+3
    EXPECT_EQ(result.getDistance(3, 0), 6);
    EXPECT_EQ(result.getDistance(1, 3), 5);  // 1->2->3 = 2+3
}

TEST_F(FloydWarshallTest, DirectedGraph) {
    GraphMatrix g(3);
    g.addDirectedEdge(0, 1, 1);
    g.addDirectedEdge(1, 2, 2);
    // Нет обратных рёбер
    
    auto result = FloydWarshall::execute(g);
    
    EXPECT_EQ(result.getDistance(0, 1), 1);
    EXPECT_EQ(result.getDistance(0, 2), 3);
    EXPECT_EQ(result.getDistance(1, 0), INF);  // Нет пути назад
    EXPECT_EQ(result.getDistance(2, 0), INF);
}

TEST_F(FloydWarshallTest, LectureExample) {
    // Граф из лекции (6 вершин)
    GraphMatrix g(6);
    g.addUndirectedEdge(0, 1, 7);   // A-B
    g.addUndirectedEdge(0, 3, 14);  // A-D
    g.addUndirectedEdge(1, 2, 9);   // B-C
    g.addUndirectedEdge(1, 3, 10);  // B-D
    g.addUndirectedEdge(1, 4, 2);   // B-E
    g.addUndirectedEdge(2, 4, 11);  // C-E
    g.addUndirectedEdge(2, 5, 6);   // C-F
    g.addUndirectedEdge(3, 4, 9);   // D-E
    g.addUndirectedEdge(4, 5, 6);   // E-F
    
    auto result = FloydWarshall::execute(g);
    
    // Проверяем расстояния от вершины 0
    EXPECT_EQ(result.getDistance(0, 0), 0);
    EXPECT_EQ(result.getDistance(0, 1), 7);
    EXPECT_EQ(result.getDistance(0, 4), 9);   // 0->1->4 = 7+2
    EXPECT_EQ(result.getDistance(0, 3), 14);
    EXPECT_EQ(result.getDistance(0, 5), 15);  // 0->1->4->5 = 7+2+6
    EXPECT_EQ(result.getDistance(0, 2), 16);  // 0->1->2 = 7+9
    
    // Проверяем симметрию для неориентированного графа
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            EXPECT_EQ(result.getDistance(i, j), result.getDistance(j, i));
        }
    }
}

// ============================================
// Тесты на восстановление пути
// ============================================

TEST_F(FloydWarshallTest, PathReconstruction) {
    GraphMatrix g(4);
    g.addUndirectedEdge(0, 1, 1);
    g.addUndirectedEdge(1, 2, 2);
    g.addUndirectedEdge(2, 3, 3);
    
    auto result = FloydWarshall::execute(g);
    auto path = result.getPath(0, 3);
    
    EXPECT_EQ(path.size(), 4);
    EXPECT_EQ(path.get(0), 0);
    EXPECT_EQ(path.get(1), 1);
    EXPECT_EQ(path.get(2), 2);
    EXPECT_EQ(path.get(3), 3);
}

TEST_F(FloydWarshallTest, PathToSelf) {
    GraphMatrix g(3);
    g.addUndirectedEdge(0, 1, 1);
    
    auto result = FloydWarshall::execute(g);
    auto path = result.getPath(0, 0);
    
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path.get(0), 0);
}

TEST_F(FloydWarshallTest, NoPath) {
    GraphMatrix g(3);
    g.addDirectedEdge(0, 1, 1);
    // Нет пути от 1 к 0 и от любой вершины к 2
    
    auto result = FloydWarshall::execute(g);
    
    auto path1 = result.getPath(1, 0);
    EXPECT_EQ(path1.size(), 0);
    
    auto path2 = result.getPath(0, 2);
    EXPECT_EQ(path2.size(), 0);
}

// ============================================
// Тесты на отрицательные веса
// ============================================

TEST_F(FloydWarshallTest, NegativeWeights) {
    GraphMatrix g(3);
    g.addDirectedEdge(0, 1, 4);
    g.addDirectedEdge(0, 2, 5);
    g.addDirectedEdge(1, 2, -3);  // Отрицательное ребро
    
    auto result = FloydWarshall::execute(g);
    
    EXPECT_EQ(result.getDistance(0, 2), 1);  // 0->1->2 = 4 + (-3) = 1 < 5
    EXPECT_FALSE(result.hasNegativeCycle);
}

TEST_F(FloydWarshallTest, NegativeCycleDetection) {
    GraphMatrix g(3);
    g.addDirectedEdge(0, 1, 1);
    g.addDirectedEdge(1, 2, -1);
    g.addDirectedEdge(2, 0, -1);  // Цикл 0->1->2->0 = 1 + (-1) + (-1) = -1
    
    auto result = FloydWarshall::execute(g);
    
    EXPECT_TRUE(result.hasNegativeCycle);
}

// ============================================
// Граничные случаи
// ============================================

TEST_F(FloydWarshallTest, DisconnectedGraph) {
    GraphMatrix g(4);
    g.addUndirectedEdge(0, 1, 1);
    g.addUndirectedEdge(2, 3, 2);
    // Две компоненты связности
    
    auto result = FloydWarshall::execute(g);
    
    EXPECT_EQ(result.getDistance(0, 1), 1);
    EXPECT_EQ(result.getDistance(2, 3), 2);
    EXPECT_EQ(result.getDistance(0, 2), INF);
    EXPECT_EQ(result.getDistance(1, 3), INF);
}

TEST_F(FloydWarshallTest, ZeroWeightEdges) {
    GraphMatrix g(3);
    g.addUndirectedEdge(0, 1, 0);
    g.addUndirectedEdge(1, 2, 0);
    
    auto result = FloydWarshall::execute(g);
    
    EXPECT_EQ(result.getDistance(0, 2), 0);
    EXPECT_FALSE(result.hasNegativeCycle);
}

TEST_F(FloydWarshallTest, SelfLoop) {
    GraphMatrix g(2);
    g.addDirectedEdge(0, 0, 5);  // Петля
    g.addUndirectedEdge(0, 1, 1);
    
    auto result = FloydWarshall::execute(g);
    
    // Расстояние до себя должно быть 0 (через "пустой" путь)
    EXPECT_EQ(result.getDistance(0, 0), 0);
    EXPECT_EQ(result.getDistance(0, 1), 1);
}

// ============================================
// Сравнение с Дейкстрой
// ============================================

TEST_F(FloydWarshallTest, CompareWithDijkstra) {
    // Тот же граф, что и в тестах Дейкстры
    GraphMatrix g(5);
    g.addUndirectedEdge(0, 1, 1);
    g.addUndirectedEdge(0, 2, 2);
    g.addUndirectedEdge(0, 3, 3);
    g.addUndirectedEdge(0, 4, 4);
    
    auto result = FloydWarshall::execute(g);
    
    // От центра (0)
    EXPECT_EQ(result.getDistance(0, 1), 1);
    EXPECT_EQ(result.getDistance(0, 2), 2);
    EXPECT_EQ(result.getDistance(0, 3), 3);
    EXPECT_EQ(result.getDistance(0, 4), 4);
    
    // От периферии (1)
    EXPECT_EQ(result.getDistance(1, 0), 1);
    EXPECT_EQ(result.getDistance(1, 2), 3);  // 1->0->2
    EXPECT_EQ(result.getDistance(1, 3), 4);  // 1->0->3
    EXPECT_EQ(result.getDistance(1, 4), 5);  // 1->0->4
}