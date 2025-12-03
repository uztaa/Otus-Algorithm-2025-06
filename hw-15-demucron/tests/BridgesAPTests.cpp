#include <gtest/gtest.h>
#include "BridgesAndAP.hpp"
#include "FactorArray.h"
#include <set>
#include <vector>
#include <algorithm>

/**
 * @brief Вспомогательная функция для создания НЕОРИЕНТИРОВАННОГО графа
 *
 * Каждое ребро добавляется в обе стороны.
 *
 * @param edges Список рёбер (каждое добавляется дважды)
 * @param vertexCount Количество вершин
 * @return SpaceArray<int> Граф в виде списка смежности
 */
SpaceArray<int> createUndirectedGraph(const std::vector<std::pair<int, int>> &edges, int vertexCount)
{
    SpaceArray<int> adjList;

    // Добавляем рёбра в обе стороны (блоки создаются автоматически)
    for (const auto &edge : edges)
    {
        adjList.add(edge.first, edge.second); // u → v
        adjList.add(edge.second, edge.first); // v → u
    }

    return adjList;
}

/**
 * @brief Нормализовать ребро (меньшая вершина первой)
 */
std::pair<int, int> normalizeEdge(const Edge &e)
{
    return {std::min(e.from, e.to), std::max(e.from, e.to)};
}

/**
 * @brief Извлечь мосты как множество для сравнения
 */
std::set<std::pair<int, int>> getBridgesAsSet(const BridgesAPResult &result)
{
    std::set<std::pair<int, int>> bridges;
    int count = result.bridges.size();
    for (int i = 0; i < count; ++i)
    {
        bridges.insert(normalizeEdge(result.bridges.get(i)));
    }
    return bridges;
}

/**
 * @brief Извлечь точки сочленения как множество
 */
std::set<int> getAPsAsSet(const BridgesAPResult &result)
{
    std::set<int> aps;
    int count = result.articulationPoints.size();
    for (int i = 0; i < count; ++i)
    {
        aps.insert(result.articulationPoints.get(i));
    }
    return aps;
}

// ============================================
// Базовые тесты
// ============================================

TEST(BridgesAPTest, EmptyGraph)
{
    SpaceArray<int> adjList;

    BridgesAPResult result = BridgesAndAP::execute(adjList, 0);

    EXPECT_EQ(result.bridges.size(), 0);
    EXPECT_EQ(result.articulationPoints.size(), 0);
}

TEST(BridgesAPTest, SingleVertex)
{
    SpaceArray<int> adjList;

    BridgesAPResult result = BridgesAndAP::execute(adjList, 1);

    EXPECT_EQ(result.bridges.size(), 0);
    EXPECT_EQ(result.articulationPoints.size(), 0);
}

TEST(BridgesAPTest, TwoVerticesOneEdge)
{
    auto adjList = createUndirectedGraph({{0, 1}}, 2);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 2);

    auto bridges = getBridgesAsSet(result);
    EXPECT_EQ(bridges, (std::set<std::pair<int, int>>{{0, 1}}));
    EXPECT_EQ(result.articulationPoints.size(), 0);
}

TEST(BridgesAPTest, Triangle)
{
    auto adjList = createUndirectedGraph({{0, 1}, {1, 2}, {2, 0}}, 3);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 3);

    EXPECT_EQ(result.bridges.size(), 0);
    EXPECT_EQ(result.articulationPoints.size(), 0);
}

TEST(BridgesAPTest, LinearGraph)
{
    auto adjList = createUndirectedGraph({{0, 1}, {1, 2}, {2, 3}}, 4);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 4);

    auto bridges = getBridgesAsSet(result);
    EXPECT_EQ(bridges, (std::set<std::pair<int, int>>{{0, 1}, {1, 2}, {2, 3}}));

    auto aps = getAPsAsSet(result);
    EXPECT_EQ(aps, (std::set<int>{1, 2}));
}

// ============================================
// Тесты на мосты
// ============================================

TEST(BridgesAPTest, BridgeBetweenTwoCycles)
{
    auto adjList = createUndirectedGraph({{0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 5}, {5, 3}, {2, 5}}, 6);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 6);

    auto bridges = getBridgesAsSet(result);
    EXPECT_EQ(bridges, (std::set<std::pair<int, int>>{{2, 5}}));

    auto aps = getAPsAsSet(result);
    EXPECT_EQ(aps, (std::set<int>{2, 5}));
}

TEST(BridgesAPTest, StarGraph)
{
    auto adjList = createUndirectedGraph({{0, 1}, {0, 2}, {0, 3}, {0, 4}}, 5);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 5);

    auto bridges = getBridgesAsSet(result);
    EXPECT_EQ(bridges.size(), 4);

    auto aps = getAPsAsSet(result);
    EXPECT_EQ(aps, (std::set<int>{0}));
}

TEST(BridgesAPTest, NoBridgesInCycle)
{
    auto adjList = createUndirectedGraph({{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}}, 5);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 5);

    EXPECT_EQ(result.bridges.size(), 0);
    EXPECT_EQ(result.articulationPoints.size(), 0);
}

// ============================================
// Тесты на точки сочленения
// ============================================

TEST(BridgesAPTest, ArticulationPointWithCycle)
{
    auto adjList = createUndirectedGraph({{0, 1}, {1, 2}, {2, 0}, {0, 3}}, 4);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 4);

    auto bridges = getBridgesAsSet(result);
    EXPECT_EQ(bridges, (std::set<std::pair<int, int>>{{0, 3}}));

    auto aps = getAPsAsSet(result);
    EXPECT_EQ(aps, (std::set<int>{0}));
}

TEST(BridgesAPTest, MultipleArticulationPoints)
{
    auto adjList = createUndirectedGraph({{0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 5}, {5, 3}, {2, 4}}, 6);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 6);

    auto bridges = getBridgesAsSet(result);
    EXPECT_EQ(bridges, (std::set<std::pair<int, int>>{{2, 4}}));

    auto aps = getAPsAsSet(result);
    EXPECT_EQ(aps, (std::set<int>{2, 4}));
}

TEST(BridgesAPTest, RootWithTwoChildren)
{
    auto adjList = createUndirectedGraph({{0, 1}, {0, 2}}, 3);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 3);

    auto bridges = getBridgesAsSet(result);
    EXPECT_EQ(bridges.size(), 2);

    auto aps = getAPsAsSet(result);
    EXPECT_EQ(aps, (std::set<int>{0}));
}

// ============================================
// Сложные графы
// ============================================

TEST(BridgesAPTest, ComplexGraph)
{
    auto adjList = createUndirectedGraph({{0, 1}, {1, 2}, {0, 3}, {1, 4}, {3, 4}, {4, 5}, {5, 6}}, 7);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 7);

    auto bridges = getBridgesAsSet(result);
    EXPECT_EQ(bridges, (std::set<std::pair<int, int>>{{1, 2}, {4, 5}, {5, 6}}));

    auto aps = getAPsAsSet(result);
    EXPECT_EQ(aps, (std::set<int>{1, 4, 5}));
}

TEST(BridgesAPTest, DisconnectedGraph)
{
    auto adjList = createUndirectedGraph({{0, 1},
                                          {2, 3},
                                          {3, 4}},
                                         5);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 5);

    auto bridges = getBridgesAsSet(result);
    EXPECT_EQ(bridges, (std::set<std::pair<int, int>>{{0, 1}, {2, 3}, {3, 4}}));

    auto aps = getAPsAsSet(result);
    EXPECT_EQ(aps, (std::set<int>{3}));
}

TEST(BridgesAPTest, DoubleEdgeNotBridge)
{
    auto adjList = createUndirectedGraph({{0, 1}, {1, 2}, {2, 0}}, 3);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 3);

    EXPECT_EQ(result.bridges.size(), 0);
    EXPECT_EQ(result.articulationPoints.size(), 0);
}

TEST(BridgesAPTest, LargeLinearGraph)
{
    const int n = 50;
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < n - 1; ++i)
    {
        edges.push_back({i, i + 1});
    }

    auto adjList = createUndirectedGraph(edges, n);

    BridgesAPResult result = BridgesAndAP::execute(adjList, n);

    EXPECT_EQ(result.bridges.size(), n - 1);
    EXPECT_EQ(result.articulationPoints.size(), n - 2);
}

TEST(BridgesAPTest, LargeCycle)
{
    const int n = 50;
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < n; ++i)
    {
        edges.push_back({i, (i + 1) % n});
    }

    auto adjList = createUndirectedGraph(edges, n);

    BridgesAPResult result = BridgesAndAP::execute(adjList, n);

    EXPECT_EQ(result.bridges.size(), 0);
    EXPECT_EQ(result.articulationPoints.size(), 0);
}

// ============================================
// Особые случаи
// ============================================

TEST(BridgesAPTest, IsolatedVertices)
{
    SpaceArray<int> adjList;

    BridgesAPResult result = BridgesAndAP::execute(adjList, 5);

    EXPECT_EQ(result.bridges.size(), 0);
    EXPECT_EQ(result.articulationPoints.size(), 0);
}

TEST(BridgesAPTest, CompleteGraph4)
{
    auto adjList = createUndirectedGraph({{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}}, 4);

    BridgesAPResult result = BridgesAndAP::execute(adjList, 4);

    EXPECT_EQ(result.bridges.size(), 0);
    EXPECT_EQ(result.articulationPoints.size(), 0);
}
