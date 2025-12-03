#include <gtest/gtest.h>
#include "TarjanSCC.hpp"
#include "FactorArray.h"
#include <set>
#include <vector>
#include <algorithm>

/**
 * @brief Вспомогательная функция для создания графа из списка рёбер
 * 
 * @param edges Список рёбер в формате {from, to}
 * @param vertexCount Количество вершин
 * @return SpaceArray<int> Граф в виде списка смежности
 */
SpaceArray<int> createDirectedGraph(const std::vector<std::pair<int, int>>& edges, int vertexCount) {
    SpaceArray<int> adjList;
    
    // Добавляем рёбра (блоки создаются автоматически)
    for (const auto& edge : edges) {
        adjList.add(edge.first, edge.second);  // from -> to
    }
    
    return adjList;
}

/**
 * @brief Извлечь компоненту как множество для удобного сравнения
 */
std::set<int> getComponentAsSet(const SpaceArray<int>& components, int componentIndex) {
    std::set<int> result;
    int size = components.blockSize(componentIndex);
    for (int i = 0; i < size; ++i) {
        result.insert(components.get(componentIndex, i));
    }
    return result;
}

// ============================================
// Базовые тесты
// ============================================

TEST(TarjanSCCTest, EmptyGraph) {
    SpaceArray<int> adjList;
    
    TarjanResult result = TarjanSCC::execute(adjList, 0);
    
    EXPECT_EQ(result.componentCount, 0);
}

TEST(TarjanSCCTest, SingleVertex) {
    SpaceArray<int> adjList;
    // Пустой граф с 1 вершиной (вершина 0 без рёбер)
    
    TarjanResult result = TarjanSCC::execute(adjList, 1);
    
    EXPECT_EQ(result.componentCount, 1);
    EXPECT_EQ(result.componentId.get(0), 0);
    
    std::set<int> component = getComponentAsSet(result.components, 0);
    EXPECT_EQ(component, std::set<int>({0}));
}

TEST(TarjanSCCTest, TwoVerticesNoEdge) {
    SpaceArray<int> adjList;
    // Пустой граф с 2 вершинами (изолированные)
    
    TarjanResult result = TarjanSCC::execute(adjList, 2);
    
    // Две изолированные вершины — две отдельные SCC
    EXPECT_EQ(result.componentCount, 2);
    EXPECT_NE(result.componentId.get(0), result.componentId.get(1));
}

TEST(TarjanSCCTest, TwoVerticesOneEdge) {
    // 0 → 1 (без обратного ребра — не SCC)
    auto adjList = createDirectedGraph({{0, 1}}, 2);
    
    TarjanResult result = TarjanSCC::execute(adjList, 2);
    
    EXPECT_EQ(result.componentCount, 2);
    EXPECT_NE(result.componentId.get(0), result.componentId.get(1));
}

TEST(TarjanSCCTest, TwoVerticesCycle) {
    // 0 → 1 → 0 (цикл — одна SCC)
    auto adjList = createDirectedGraph({{0, 1}, {1, 0}}, 2);
    
    TarjanResult result = TarjanSCC::execute(adjList, 2);
    
    EXPECT_EQ(result.componentCount, 1);
    EXPECT_EQ(result.componentId.get(0), result.componentId.get(1));
    
    std::set<int> component = getComponentAsSet(result.components, 0);
    EXPECT_EQ(component, std::set<int>({0, 1}));
}

TEST(TarjanSCCTest, SelfLoop) {
    // 0 → 0 (самопетля — одна SCC)
    auto adjList = createDirectedGraph({{0, 0}}, 1);
    
    TarjanResult result = TarjanSCC::execute(adjList, 1);
    
    EXPECT_EQ(result.componentCount, 1);
}

// ============================================
// Тесты на классические примеры
// ============================================

TEST(TarjanSCCTest, ThreeVertexCycle) {
    // 0 → 1 → 2 → 0 (треугольник)
    auto adjList = createDirectedGraph({{0, 1}, {1, 2}, {2, 0}}, 3);
    
    TarjanResult result = TarjanSCC::execute(adjList, 3);
    
    EXPECT_EQ(result.componentCount, 1);
    
    std::set<int> component = getComponentAsSet(result.components, 0);
    EXPECT_EQ(component, std::set<int>({0, 1, 2}));
}

TEST(TarjanSCCTest, LinearGraph) {
    // 0 → 1 → 2 → 3 (линейный граф без циклов)
    auto adjList = createDirectedGraph({{0, 1}, {1, 2}, {2, 3}}, 4);
    
    TarjanResult result = TarjanSCC::execute(adjList, 4);
    
    // Каждая вершина — отдельная SCC
    EXPECT_EQ(result.componentCount, 4);
    
    // Все вершины в разных компонентах
    std::set<int> componentIds;
    for (int i = 0; i < 4; ++i) {
        componentIds.insert(result.componentId.get(i));
    }
    EXPECT_EQ(componentIds.size(), 4);
}

TEST(TarjanSCCTest, TwoSCCs) {
    // Две компоненты: {0, 1} и {2, 3}
    // 0 ↔ 1 → 2 ↔ 3
    auto adjList = createDirectedGraph({
        {0, 1}, {1, 0},  // SCC 1
        {1, 2},          // Ребро между SCC
        {2, 3}, {3, 2}   // SCC 2
    }, 4);
    
    TarjanResult result = TarjanSCC::execute(adjList, 4);
    
    EXPECT_EQ(result.componentCount, 2);
    
    // Вершины 0 и 1 в одной компоненте
    EXPECT_EQ(result.componentId.get(0), result.componentId.get(1));
    
    // Вершины 2 и 3 в одной компоненте
    EXPECT_EQ(result.componentId.get(2), result.componentId.get(3));
    
    // Разные компоненты
    EXPECT_NE(result.componentId.get(0), result.componentId.get(2));
}

TEST(TarjanSCCTest, ClassicExample) {
    // Классический пример с 8 вершинами
    //     0 → 1 → 2
    //     ↑   ↓   ↓
    //     4 ← 3   5 ↔ 6
    //             ↓
    //             7
    //
    // SCC: {0,1,3,4}, {2}, {5,6}, {7}
    
    auto adjList = createDirectedGraph({
        {0, 1},
        {1, 2}, {1, 3},
        {2, 5},
        {3, 4},
        {4, 0},
        {5, 6}, {5, 7},
        {6, 5}
    }, 8);
    
    TarjanResult result = TarjanSCC::execute(adjList, 8);
    
    EXPECT_EQ(result.componentCount, 4);
    
    // Проверяем, что вершины в правильных компонентах
    EXPECT_EQ(result.componentId.get(0), result.componentId.get(1));
    EXPECT_EQ(result.componentId.get(1), result.componentId.get(3));
    EXPECT_EQ(result.componentId.get(3), result.componentId.get(4));
    
    EXPECT_EQ(result.componentId.get(5), result.componentId.get(6));
    
    // Вершины 2 и 7 — отдельные SCC
    EXPECT_NE(result.componentId.get(2), result.componentId.get(0));
    EXPECT_NE(result.componentId.get(2), result.componentId.get(5));
    EXPECT_NE(result.componentId.get(7), result.componentId.get(5));
}

TEST(TarjanSCCTest, DisconnectedComponents) {
    // Два изолированных цикла
    // Компонента 1: 0 → 1 → 0
    // Компонента 2: 2 → 3 → 2
    
    auto adjList = createDirectedGraph({
        {0, 1}, {1, 0},
        {2, 3}, {3, 2}
    }, 4);
    
    TarjanResult result = TarjanSCC::execute(adjList, 4);
    
    EXPECT_EQ(result.componentCount, 2);
    
    EXPECT_EQ(result.componentId.get(0), result.componentId.get(1));
    EXPECT_EQ(result.componentId.get(2), result.componentId.get(3));
    EXPECT_NE(result.componentId.get(0), result.componentId.get(2));
}

TEST(TarjanSCCTest, ComplexGraphWithMultipleSCCs) {
    // Граф с 5 компонентами разного размера
    //
    //   0 ─→ 1 ─→ 2
    //   ↑    ↓    
    //   └─── 3    4 (изолированная)
    //
    // SCC: {0,1,3}, {2}, {4}
    
    auto adjList = createDirectedGraph({
        {0, 1},
        {1, 2}, {1, 3},
        {3, 0}
    }, 5);
    
    TarjanResult result = TarjanSCC::execute(adjList, 5);
    
    EXPECT_EQ(result.componentCount, 3);
    
    // {0, 1, 3} в одной компоненте
    EXPECT_EQ(result.componentId.get(0), result.componentId.get(1));
    EXPECT_EQ(result.componentId.get(1), result.componentId.get(3));
    
    // 2 и 4 — отдельные компоненты
    EXPECT_NE(result.componentId.get(2), result.componentId.get(0));
    EXPECT_NE(result.componentId.get(4), result.componentId.get(0));
    EXPECT_NE(result.componentId.get(4), result.componentId.get(2));
}

// ============================================
// Тесты корректности результата
// ============================================

TEST(TarjanSCCTest, AllVerticesAssigned) {
    auto adjList = createDirectedGraph({
        {0, 1}, {1, 2}, {2, 0},
        {3, 4}, {4, 3}
    }, 5);
    
    TarjanResult result = TarjanSCC::execute(adjList, 5);
    
    // Все вершины должны быть назначены компонентам
    for (int i = 0; i < 5; ++i) {
        EXPECT_GE(result.componentId.get(i), 0);
        EXPECT_LT(result.componentId.get(i), result.componentCount);
    }
}

TEST(TarjanSCCTest, ComponentsContainAllVertices) {
    auto adjList = createDirectedGraph({
        {0, 1}, {1, 0},
        {2, 3}, {3, 2},
        {4, 4}  // самопетля
    }, 5);
    
    TarjanResult result = TarjanSCC::execute(adjList, 5);
    
    // Собираем все вершины из всех компонент
    std::set<int> allVertices;
    for (int c = 0; c < result.componentCount; ++c) {
        int size = result.components.blockSize(c);
        for (int i = 0; i < size; ++i) {
            allVertices.insert(result.components.get(c, i));
        }
    }
    
    EXPECT_EQ(allVertices, std::set<int>({0, 1, 2, 3, 4}));
}

TEST(TarjanSCCTest, LargeCycle) {
    // Цикл из 100 вершин — одна SCC
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < 100; ++i) {
        edges.push_back({i, (i + 1) % 100});
    }
    
    auto adjList = createDirectedGraph(edges, 100);
    
    TarjanResult result = TarjanSCC::execute(adjList, 100);
    
    EXPECT_EQ(result.componentCount, 1);
    
    // Все вершины в одной компоненте
    int firstComponentId = result.componentId.get(0);
    for (int i = 1; i < 100; ++i) {
        EXPECT_EQ(result.componentId.get(i), firstComponentId);
    }
}