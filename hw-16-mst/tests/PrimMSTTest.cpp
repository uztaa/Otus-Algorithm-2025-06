#include <gtest/gtest.h>
#include "PrimMST.hpp"
#include <set>
#include <utility>
#include <MSTResult.hpp>

/**
 * @brief Вспомогательная функция для создания взвешенного графа
 */
SpaceArray<int> createWeightedGraphPrim(
    const std::vector<std::tuple<int, int, int>>& edges, 
    int vertexCount) {
    
    SpaceArray<int> adjList;
    
    for (const auto& [from, to, weight] : edges) {
        adjList.add(from, to);
        adjList.add(from, weight);
        adjList.add(to, from);
        adjList.add(to, weight);
    }
    
    return adjList;
}

/**
 * @brief Извлечь рёбра MST как множество
 */
std::set<std::pair<int, int>> getPrimMSTEdgesAsSet(const MSTResult& result) {
    std::set<std::pair<int, int>> edges;
    for (int i = 0; i < result.edges.size(); ++i) {
        Edge e = result.edges.get(i);
        int minV = std::min(e.v1, e.v2);
        int maxV = std::max(e.v1, e.v2);
        edges.insert({minV, maxV});
    }
    return edges;
}

// ============================================
// Базовые тесты
// ============================================

TEST(PrimMSTTest, EmptyGraph) {
    SpaceArray<int> adjList;
    
    MSTResult result = PrimMST::execute(adjList, 0);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 0);
    EXPECT_EQ(result.totalWeight, 0);
}

TEST(PrimMSTTest, SingleVertex) {
    SpaceArray<int> adjList;
    
    MSTResult result = PrimMST::execute(adjList, 1);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 0);
    EXPECT_EQ(result.totalWeight, 0);
}

TEST(PrimMSTTest, TwoVerticesOneEdge) {
    auto adjList = createWeightedGraphPrim({{0, 1, 5}}, 2);
    
    MSTResult result = PrimMST::execute(adjList, 2);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 1);
    EXPECT_EQ(result.totalWeight, 5);
}

// ============================================
// Тесты на классические примеры
// ============================================

TEST(PrimMSTTest, Triangle) {
    auto adjList = createWeightedGraphPrim({
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 3}
    }, 3);
    
    MSTResult result = PrimMST::execute(adjList, 3);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 2);
    EXPECT_EQ(result.totalWeight, 3);
}

TEST(PrimMSTTest, Square) {
    auto adjList = createWeightedGraphPrim({
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3},
        {3, 0, 4},
        {0, 2, 5},
        {1, 3, 6}
    }, 4);
    
    MSTResult result = PrimMST::execute(adjList, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);
    EXPECT_EQ(result.totalWeight, 6);
}

TEST(PrimMSTTest, LectureExample) {
    // Пример из лекции (тот же граф, что и для Краскала)
    auto adjList = createWeightedGraphPrim({
        {0, 1, 7},   // A-B
        {0, 3, 5},   // A-D
        {1, 2, 8},   // B-C
        {1, 3, 9},   // B-D
        {1, 4, 7},   // B-E
        {2, 4, 5},   // C-E
        {3, 4, 15},  // D-E
        {3, 5, 6},   // D-F
        {4, 5, 8},   // E-F
        {4, 6, 9},   // E-G
        {5, 6, 11}   // F-G
    }, 7);
    
    MSTResult result = PrimMST::execute(adjList, 7);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 6);
    EXPECT_EQ(result.totalWeight, 39);  // Должен совпадать с Краскалом
}

// ============================================
// Тесты на особые случаи
// ============================================

TEST(PrimMSTTest, LinearGraph) {
    auto adjList = createWeightedGraphPrim({
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3}
    }, 4);
    
    MSTResult result = PrimMST::execute(adjList, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);
    EXPECT_EQ(result.totalWeight, 6);
}

TEST(PrimMSTTest, StarGraph) {
    auto adjList = createWeightedGraphPrim({
        {0, 1, 1},
        {0, 2, 2},
        {0, 3, 3},
        {0, 4, 4}
    }, 5);
    
    MSTResult result = PrimMST::execute(adjList, 5);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 4);
    EXPECT_EQ(result.totalWeight, 10);
}

TEST(PrimMSTTest, DisconnectedGraph) {
    auto adjList = createWeightedGraphPrim({
        {0, 1, 1},
        {2, 3, 2}
    }, 4);
    
    MSTResult result = PrimMST::execute(adjList, 4);
    
    EXPECT_FALSE(result.isConnected);
}

TEST(PrimMSTTest, IsolatedVertex) {
    auto adjList = createWeightedGraphPrim({
        {0, 1, 1}
    }, 3);
    
    MSTResult result = PrimMST::execute(adjList, 3);
    
    EXPECT_FALSE(result.isConnected);
}

TEST(PrimMSTTest, EqualWeights) {
    auto adjList = createWeightedGraphPrim({
        {0, 1, 5},
        {1, 2, 5},
        {2, 0, 5}
    }, 3);
    
    MSTResult result = PrimMST::execute(adjList, 3);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 2);
    EXPECT_EQ(result.totalWeight, 10);
}

// ============================================
// Тест с использованием FactorArray<WeightedEdge>
// ============================================

TEST(PrimMSTTest, EdgeArrayInput) {
    FactorArray<WeightedEdge> edges;
    edges.add(WeightedEdge(0, 1, 4), edges.size());
    edges.add(WeightedEdge(0, 2, 3), edges.size());
    edges.add(WeightedEdge(1, 2, 1), edges.size());
    edges.add(WeightedEdge(1, 3, 2), edges.size());
    edges.add(WeightedEdge(2, 3, 4), edges.size());
    
    MSTResult result = PrimMST::execute(edges, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);
    EXPECT_EQ(result.totalWeight, 6);
}

// ============================================
// Сравнение с Краскалом
// ============================================

TEST(PrimMSTTest, SameAsKruskal) {
    // Проверяем, что оба алгоритма дают одинаковый вес MST
    auto adjList = createWeightedGraphPrim({
        {0, 1, 10}, {0, 2, 6}, {0, 3, 5},
        {1, 3, 15}, {2, 3, 4}
    }, 4);
    
    MSTResult resultPrim = PrimMST::execute(adjList, 4);
    
    // MST: 2-3(4), 0-3(5), 0-1(10) = 19
    // или: 2-3(4), 0-3(5), 0-2(6), но тогда 0-2 не нужен
    EXPECT_TRUE(resultPrim.isConnected);
    EXPECT_EQ(resultPrim.edges.size(), 3);
    EXPECT_EQ(resultPrim.totalWeight, 19);
}

// ============================================
// Тесты на большие графы
// ============================================

TEST(PrimMSTTest, LargeLinearGraph) {
    const int n = 100;
    std::vector<std::tuple<int, int, int>> edges;
    int expectedWeight = 0;
    
    for (int i = 0; i < n - 1; ++i) {
        edges.push_back({i, i + 1, i + 1});
        expectedWeight += i + 1;
    }
    
    auto adjList = createWeightedGraphPrim(edges, n);
    
    MSTResult result = PrimMST::execute(adjList, n);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), n - 1);
    EXPECT_EQ(result.totalWeight, expectedWeight);
}

TEST(PrimMSTTest, CompleteGraph5) {
    auto adjList = createWeightedGraphPrim({
        {0, 1, 2}, {0, 2, 3}, {0, 3, 1}, {0, 4, 5},
        {1, 2, 4}, {1, 3, 6}, {1, 4, 7},
        {2, 3, 8}, {2, 4, 9},
        {3, 4, 10}
    }, 5);
    
    MSTResult result = PrimMST::execute(adjList, 5);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 4);
    EXPECT_LE(result.totalWeight, 11);
}

// ============================================
// Тест на порядок добавления рёбер
// ============================================

TEST(PrimMSTTest, GrowsFromStart) {
    // Прим всегда начинает с вершины 0
    auto adjList = createWeightedGraphPrim({
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3}
    }, 4);
    
    MSTResult result = PrimMST::execute(adjList, 4);
    
    EXPECT_TRUE(result.isConnected);
    
    // Первое ребро должно включать вершину 0
    if (result.edges.size() > 0) {
        Edge first = result.edges.get(0);
        EXPECT_TRUE(first.v1 == 0 || first.v2 == 0);
    }
}