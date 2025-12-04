#include <gtest/gtest.h>
#include "BoruvkaMST.hpp"
#include <set>
#include <utility>

/**
 * @brief Вспомогательная функция для создания взвешенного графа
 */
SpaceArray<int> createWeightedGraphBoruvka(
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
std::set<std::pair<int, int>> getBoruvkaMSTEdgesAsSet(const MSTResult& result) {
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

TEST(BoruvkaMSTTest, EmptyGraph) {
    SpaceArray<int> adjList;
    
    MSTResult result = BoruvkaMST::execute(adjList, 0);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 0);
    EXPECT_EQ(result.totalWeight, 0);
}

TEST(BoruvkaMSTTest, SingleVertex) {
    SpaceArray<int> adjList;
    
    MSTResult result = BoruvkaMST::execute(adjList, 1);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 0);
    EXPECT_EQ(result.totalWeight, 0);
}

TEST(BoruvkaMSTTest, TwoVerticesOneEdge) {
    auto adjList = createWeightedGraphBoruvka({{0, 1, 5}}, 2);
    
    MSTResult result = BoruvkaMST::execute(adjList, 2);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 1);
    EXPECT_EQ(result.totalWeight, 5);
}

// ============================================
// Тесты на классические примеры
// ============================================

TEST(BoruvkaMSTTest, Triangle) {
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 3}
    }, 3);
    
    MSTResult result = BoruvkaMST::execute(adjList, 3);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 2);
    EXPECT_EQ(result.totalWeight, 3);
}

TEST(BoruvkaMSTTest, Square) {
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3},
        {3, 0, 4},
        {0, 2, 5},
        {1, 3, 6}
    }, 4);
    
    MSTResult result = BoruvkaMST::execute(adjList, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);
    EXPECT_EQ(result.totalWeight, 6);
}

TEST(BoruvkaMSTTest, LectureExample) {
    auto adjList = createWeightedGraphBoruvka({
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
    
    MSTResult result = BoruvkaMST::execute(adjList, 7);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 6);
    EXPECT_EQ(result.totalWeight, 39);  // Должен совпадать с Краскалом и Примом
}

// ============================================
// Тесты на особые случаи
// ============================================

TEST(BoruvkaMSTTest, LinearGraph) {
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3}
    }, 4);
    
    MSTResult result = BoruvkaMST::execute(adjList, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);
    EXPECT_EQ(result.totalWeight, 6);
}

TEST(BoruvkaMSTTest, StarGraph) {
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 1},
        {0, 2, 2},
        {0, 3, 3},
        {0, 4, 4}
    }, 5);
    
    MSTResult result = BoruvkaMST::execute(adjList, 5);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 4);
    EXPECT_EQ(result.totalWeight, 10);
}

TEST(BoruvkaMSTTest, DisconnectedGraph) {
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 1},
        {2, 3, 2}
    }, 4);
    
    MSTResult result = BoruvkaMST::execute(adjList, 4);
    
    EXPECT_FALSE(result.isConnected);
}

TEST(BoruvkaMSTTest, IsolatedVertex) {
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 1}
    }, 3);
    
    MSTResult result = BoruvkaMST::execute(adjList, 3);
    
    EXPECT_FALSE(result.isConnected);
}

TEST(BoruvkaMSTTest, EqualWeights) {
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 5},
        {1, 2, 5},
        {2, 0, 5}
    }, 3);
    
    MSTResult result = BoruvkaMST::execute(adjList, 3);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 2);
    EXPECT_EQ(result.totalWeight, 10);
}

// ============================================
// Тест с использованием FactorArray<WeightedEdge>
// ============================================

TEST(BoruvkaMSTTest, EdgeArrayInput) {
    FactorArray<WeightedEdge> edges;
    edges.add(WeightedEdge(0, 1, 4), edges.size());
    edges.add(WeightedEdge(0, 2, 3), edges.size());
    edges.add(WeightedEdge(1, 2, 1), edges.size());
    edges.add(WeightedEdge(1, 3, 2), edges.size());
    edges.add(WeightedEdge(2, 3, 4), edges.size());
    
    MSTResult result = BoruvkaMST::execute(edges, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);
    EXPECT_EQ(result.totalWeight, 6);
}

// ============================================
// Проверка параллельности Борувки
// ============================================

TEST(BoruvkaMSTTest, MultipleComponentsMerge) {
    // Граф, где на первой итерации объединятся несколько компонент
    // Каждая вершина сначала — отдельная компонента
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 1},  // Компонента 0 и 1 объединятся
        {2, 3, 1},  // Компонента 2 и 3 объединятся
        {4, 5, 1},  // Компонента 4 и 5 объединятся
        {1, 2, 2},  // Соединяет полученные компоненты
        {3, 4, 2}   // Соединяет полученные компоненты
    }, 6);
    
    MSTResult result = BoruvkaMST::execute(adjList, 6);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 5);
    EXPECT_EQ(result.totalWeight, 7);  // 1+1+1+2+2
}

// ============================================
// Сравнение всех трёх алгоритмов
// ============================================

TEST(BoruvkaMSTTest, SameAsKruskalAndPrim) {
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 10}, {0, 2, 6}, {0, 3, 5},
        {1, 3, 15}, {2, 3, 4}
    }, 4);
    
    MSTResult result = BoruvkaMST::execute(adjList, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);
    EXPECT_EQ(result.totalWeight, 19);
}

// ============================================
// Тесты на большие графы
// ============================================

TEST(BoruvkaMSTTest, LargeLinearGraph) {
    const int n = 100;
    std::vector<std::tuple<int, int, int>> edges;
    int expectedWeight = 0;
    
    for (int i = 0; i < n - 1; ++i) {
        edges.push_back({i, i + 1, i + 1});
        expectedWeight += i + 1;
    }
    
    auto adjList = createWeightedGraphBoruvka(edges, n);
    
    MSTResult result = BoruvkaMST::execute(adjList, n);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), n - 1);
    EXPECT_EQ(result.totalWeight, expectedWeight);
}

TEST(BoruvkaMSTTest, CompleteGraph5) {
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 2}, {0, 2, 3}, {0, 3, 1}, {0, 4, 5},
        {1, 2, 4}, {1, 3, 6}, {1, 4, 7},
        {2, 3, 8}, {2, 4, 9},
        {3, 4, 10}
    }, 5);
    
    MSTResult result = BoruvkaMST::execute(adjList, 5);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 4);
    EXPECT_LE(result.totalWeight, 11);
}

// ============================================
// Тест на логарифмическое число итераций
// ============================================

TEST(BoruvkaMSTTest, LogarithmicIterations) {
    // Для 8 вершин должно быть не более 3 итераций (log2(8) = 3)
    auto adjList = createWeightedGraphBoruvka({
        {0, 1, 1}, {2, 3, 1}, {4, 5, 1}, {6, 7, 1},
        {1, 2, 2}, {5, 6, 2},
        {3, 4, 3}
    }, 8);
    
    MSTResult result = BoruvkaMST::execute(adjList, 8);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 7);
    EXPECT_EQ(result.totalWeight, 11);  // 1*4 + 2*2 + 3*1
}