#include <gtest/gtest.h>
#include "KruskalMST.hpp"
#include <set>
#include <utility>

/**
 * @brief Вспомогательная функция для создания взвешенного графа
 * 
 * @param edges Список рёбер {from, to, weight}
 * @param vertexCount Количество вершин
 * @return SpaceArray<int> Граф в формате списка смежности
 */
SpaceArray<int> createWeightedGraph(
    const std::vector<std::tuple<int, int, int>>& edges, 
    int vertexCount) {
    
    SpaceArray<int> adjList;
    
    for (const auto& [from, to, weight] : edges) {
        // Неориентированный граф: добавляем в обе стороны
        adjList.add(from, to);
        adjList.add(from, weight);
        adjList.add(to, from);
        adjList.add(to, weight);
    }
    
    return adjList;
}

/**
 * @brief Извлечь рёбра MST как множество для сравнения
 */
std::set<std::pair<int, int>> getMSTEdgesAsSet(const MSTResult& result) {
    std::set<std::pair<int, int>> edges;
    for (int i = 0; i < result.edges.size(); ++i) {
        Edge e = result.edges.get(i);
        // Нормализуем: меньшая вершина первой
        int minV = std::min(e.v1, e.v2);
        int maxV = std::max(e.v1, e.v2);
        edges.insert({minV, maxV});
    }
    return edges;
}

// ============================================
// Базовые тесты
// ============================================

TEST(KruskalMSTTest, EmptyGraph) {
    SpaceArray<int> adjList;
    
    MSTResult result = KruskalMST::execute(adjList, 0);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 0);
    EXPECT_EQ(result.totalWeight, 0);
}

TEST(KruskalMSTTest, SingleVertex) {
    SpaceArray<int> adjList;
    
    MSTResult result = KruskalMST::execute(adjList, 1);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 0);
    EXPECT_EQ(result.totalWeight, 0);
}

TEST(KruskalMSTTest, TwoVerticesOneEdge) {
    auto adjList = createWeightedGraph({{0, 1, 5}}, 2);
    
    MSTResult result = KruskalMST::execute(adjList, 2);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 1);
    EXPECT_EQ(result.totalWeight, 5);
    
    auto edges = getMSTEdgesAsSet(result);
    EXPECT_EQ(edges, (std::set<std::pair<int, int>>{{0, 1}}));
}

// ============================================
// Тесты на классические примеры
// ============================================

TEST(KruskalMSTTest, Triangle) {
    // Треугольник: 0-1 (w=1), 1-2 (w=2), 0-2 (w=3)
    // MST: 0-1, 1-2 (вес = 3)
    auto adjList = createWeightedGraph({
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 3}
    }, 3);
    
    MSTResult result = KruskalMST::execute(adjList, 3);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 2);
    EXPECT_EQ(result.totalWeight, 3);
    
    auto edges = getMSTEdgesAsSet(result);
    EXPECT_EQ(edges, (std::set<std::pair<int, int>>{{0, 1}, {1, 2}}));
}

TEST(KruskalMSTTest, Square) {
    // Квадрат с диагоналями:
    // 0 -- 1
    // |  X |
    // 3 -- 2
    // Рёбра: 0-1(1), 1-2(2), 2-3(3), 3-0(4), 0-2(5), 1-3(6)
    // MST: 0-1, 1-2, 2-3 (вес = 6)
    auto adjList = createWeightedGraph({
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3},
        {3, 0, 4},
        {0, 2, 5},
        {1, 3, 6}
    }, 4);
    
    MSTResult result = KruskalMST::execute(adjList, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);
    EXPECT_EQ(result.totalWeight, 6);
}

TEST(KruskalMSTTest, LectureExample) {
    // Пример из лекции (граф с 7 вершинами A-G)
    // A=0, B=1, C=2, D=3, E=4, F=5, G=6
    // Рёбра: A-B(7), A-D(5), B-C(8), B-D(9), B-E(7), 
    //        C-E(5), D-E(15), D-F(6), E-F(8), E-G(9), F-G(11)
    auto adjList = createWeightedGraph({
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
    
    MSTResult result = KruskalMST::execute(adjList, 7);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 6);  // 7 вершин - 1
    EXPECT_EQ(result.totalWeight, 39);  // 5+5+6+7+7+9
}

// ============================================
// Тесты на особые случаи
// ============================================

TEST(KruskalMSTTest, LinearGraph) {
    // Линейный граф: 0 - 1 - 2 - 3
    auto adjList = createWeightedGraph({
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3}
    }, 4);
    
    MSTResult result = KruskalMST::execute(adjList, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);
    EXPECT_EQ(result.totalWeight, 6);
}

TEST(KruskalMSTTest, StarGraph) {
    // Звезда: центр 0 соединён со всеми
    auto adjList = createWeightedGraph({
        {0, 1, 1},
        {0, 2, 2},
        {0, 3, 3},
        {0, 4, 4}
    }, 5);
    
    MSTResult result = KruskalMST::execute(adjList, 5);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 4);
    EXPECT_EQ(result.totalWeight, 10);
}

TEST(KruskalMSTTest, DisconnectedGraph) {
    // Несвязный граф: 0-1 и 2-3 отдельно
    auto adjList = createWeightedGraph({
        {0, 1, 1},
        {2, 3, 2}
    }, 4);
    
    MSTResult result = KruskalMST::execute(adjList, 4);
    
    EXPECT_FALSE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 2);  // Построено 2 ребра
}

TEST(KruskalMSTTest, IsolatedVertex) {
    // Граф с изолированной вершиной
    auto adjList = createWeightedGraph({
        {0, 1, 1}
    }, 3);
    
    MSTResult result = KruskalMST::execute(adjList, 3);
    
    EXPECT_FALSE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 1);
}

TEST(KruskalMSTTest, EqualWeights) {
    // Все рёбра одинакового веса
    auto adjList = createWeightedGraph({
        {0, 1, 5},
        {1, 2, 5},
        {2, 0, 5}
    }, 3);
    
    MSTResult result = KruskalMST::execute(adjList, 3);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 2);
    EXPECT_EQ(result.totalWeight, 10);
}

// ============================================
// Тест с использованием FactorArray<WeightedEdge>
// ============================================

TEST(KruskalMSTTest, EdgeArrayInput) {
    FactorArray<WeightedEdge> edges;
    edges.add(WeightedEdge(0, 1, 4), edges.size());
    edges.add(WeightedEdge(0, 2, 3), edges.size());
    edges.add(WeightedEdge(1, 2, 1), edges.size());
    edges.add(WeightedEdge(1, 3, 2), edges.size());
    edges.add(WeightedEdge(2, 3, 4), edges.size());
    
    MSTResult result = KruskalMST::execute(edges, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);
    EXPECT_EQ(result.totalWeight, 6);  // 1 + 2 + 3
}

// ============================================
// Тесты на большие графы
// ============================================

TEST(KruskalMSTTest, LargeLinearGraph) {
    const int n = 100;
    std::vector<std::tuple<int, int, int>> edges;
    int expectedWeight = 0;
    
    for (int i = 0; i < n - 1; ++i) {
        edges.push_back({i, i + 1, i + 1});
        expectedWeight += i + 1;
    }
    
    auto adjList = createWeightedGraph(edges, n);
    
    MSTResult result = KruskalMST::execute(adjList, n);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), n - 1);
    EXPECT_EQ(result.totalWeight, expectedWeight);
}

TEST(KruskalMSTTest, CompleteGraph5) {
    // Полный граф K5 со случайными весами
    auto adjList = createWeightedGraph({
        {0, 1, 2}, {0, 2, 3}, {0, 3, 1}, {0, 4, 5},
        {1, 2, 4}, {1, 3, 6}, {1, 4, 7},
        {2, 3, 8}, {2, 4, 9},
        {3, 4, 10}
    }, 5);
    
    MSTResult result = KruskalMST::execute(adjList, 5);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 4);  // 5 вершин - 1
    // MST: 0-3(1), 0-1(2), 0-2(3), 0-4(5) = 11
    // или другой вариант с тем же весом
    EXPECT_LE(result.totalWeight, 11);
}

// ============================================
// Тест на корректность MST
// ============================================

TEST(KruskalMSTTest, MSTProperty) {
    // Проверяем, что результат действительно дерево:
    // - V-1 рёбер
    // - Все вершины связаны
    
    auto adjList = createWeightedGraph({
        {0, 1, 1}, {0, 2, 2}, {0, 3, 3},
        {1, 2, 4}, {1, 3, 5},
        {2, 3, 6}
    }, 4);
    
    MSTResult result = KruskalMST::execute(adjList, 4);
    
    EXPECT_TRUE(result.isConnected);
    EXPECT_EQ(result.edges.size(), 3);  // V-1 рёбер
    
    // Проверяем, что все вершины участвуют
    std::set<int> vertices;
    for (int i = 0; i < result.edges.size(); ++i) {
        Edge e = result.edges.get(i);
        vertices.insert(e.v1);
        vertices.insert(e.v2);
    }
    EXPECT_EQ(vertices.size(), 4);
}