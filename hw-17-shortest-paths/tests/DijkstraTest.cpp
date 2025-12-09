#include <gtest/gtest.h>
#include "Dijkstra.hpp"
#include "GraphMatrix.hpp"

// ============================================
// Вспомогательные функции
// ============================================

/**
 * @brief Создать неориентированный граф из списка рёбер
 */
GraphMatrix createUndirectedGraph(int n, 
    const std::vector<std::tuple<int, int, int>>& edges) {
    GraphMatrix g(n);
    for (const auto& [u, v, w] : edges) {
        g.addUndirectedEdge(u, v, w);
    }
    return g;
}

/**
 * @brief Создать ориентированный граф из списка рёбер
 */
GraphMatrix createDirectedGraph(int n,
    const std::vector<std::tuple<int, int, int>>& edges) {
    GraphMatrix g(n);
    for (const auto& [u, v, w] : edges) {
        g.addDirectedEdge(u, v, w);
    }
    return g;
}

/**
 * @brief Проверить, что путь корректен (последовательные рёбра существуют)
 */
bool isValidPath(const GraphMatrix& g, const FactorArray<int>& path) {
    if (path.size() <= 1) return true;
    
    for (int i = 0; i < path.size() - 1; ++i) {
        if (!g.hasEdge(path.get(i), path.get(i + 1))) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Вычислить длину пути
 */
int computePathLength(const GraphMatrix& g, const FactorArray<int>& path) {
    if (path.size() <= 1) return 0;
    
    int length = 0;
    for (int i = 0; i < path.size() - 1; ++i) {
        length += g.getWeight(path.get(i), path.get(i + 1));
    }
    return length;
}

// ============================================
// Базовые тесты
// ============================================

TEST(DijkstraTest, EmptyGraph) {
    GraphMatrix g(0);
    // Должно выбросить исключение для любого source
    EXPECT_THROW(Dijkstra::execute(g, 0), std::out_of_range);
}

TEST(DijkstraTest, SingleVertex) {
    GraphMatrix g(1);
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_EQ(result.sourceVertex, 0);
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.dist.size(), 1);
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.edges.size(), 0);  // Нет рёбер
}

TEST(DijkstraTest, TwoVerticesConnected) {
    auto g = createUndirectedGraph(2, {
        {0, 1, 5}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_EQ(result.sourceVertex, 0);
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.dist.get(1), 5);
    EXPECT_EQ(result.edges.size(), 1);
}

TEST(DijkstraTest, TwoVerticesDisconnected) {
    GraphMatrix g(2);  // Нет рёбер
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_FALSE(result.allReachable);
    EXPECT_EQ(result.edges.size(), 0);  // Вершина 1 недостижима
}

TEST(DijkstraTest, InvalidSource) {
    GraphMatrix g(3);
    
    EXPECT_THROW(Dijkstra::execute(g, -1), std::out_of_range);
    EXPECT_THROW(Dijkstra::execute(g, 3), std::out_of_range);
    EXPECT_THROW(Dijkstra::execute(g, 100), std::out_of_range);
}

// ============================================
// Тесты на простых графах
// ============================================

TEST(DijkstraTest, Triangle) {
    //     0
    //    /|\
    //   1 | 3
    //  /  |  \
    // 1---2---2
    //     2
    auto g = createUndirectedGraph(3, {
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 3}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.dist.get(1), 1);
    EXPECT_EQ(result.dist.get(2), 3);  // Кратчайший путь 0->1->2 = 3, а не 0->2 = 3
    
    // Дерево должно содержать 2 ребра
    EXPECT_EQ(result.edges.size(), 2);
}

TEST(DijkstraTest, LinearGraph) {
    // 0 --1-- 1 --2-- 2 --3-- 3
    auto g = createUndirectedGraph(4, {
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.dist.get(1), 1);
    EXPECT_EQ(result.dist.get(2), 3);
    EXPECT_EQ(result.dist.get(3), 6);
    
    EXPECT_EQ(result.edges.size(), 3);
}

TEST(DijkstraTest, StarGraph) {
    //     1
    //     |1
    // 4-4-0-2-2
    //     |3
    //     3
    auto g = createUndirectedGraph(5, {
        {0, 1, 1},
        {0, 2, 2},
        {0, 3, 3},
        {0, 4, 4}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.dist.get(1), 1);
    EXPECT_EQ(result.dist.get(2), 2);
    EXPECT_EQ(result.dist.get(3), 3);
    EXPECT_EQ(result.dist.get(4), 4);
    
    EXPECT_EQ(result.edges.size(), 4);
}

// ============================================
// Тест из лекции
// ============================================

TEST(DijkstraTest, LectureExample) {
    /**
     * Граф из лекции:
     *    7     9
     * A --- B --- C
     * |     | \   |
     * 14    10  2  11
     * |     |    \ |
     * D ---- E ---- F
     *    9       6
     * 
     * Вершины: A=0, B=1, C=2, D=3, E=4, F=5
     */
    auto g = createUndirectedGraph(6, {
        {0, 1, 7},   // A-B
        {0, 3, 14},  // A-D (в лекции было 14, не 9)
        {1, 2, 9},   // B-C
        {1, 3, 10},  // B-D (добавляем для полноты, если было)
        {1, 4, 2},   // B-E
        {2, 4, 11},  // C-E
        {2, 5, 6},   // C-F (добавил вместо E-F)
        {3, 4, 9},   // D-E
        {4, 5, 6}    // E-F
    });
    
    auto result = Dijkstra::execute(g, 0);  // От A
    
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.dist.get(0), 0);   // A
    EXPECT_EQ(result.dist.get(1), 7);   // B: A->B = 7
    EXPECT_EQ(result.dist.get(4), 9);   // E: A->B->E = 7+2 = 9
    
    // Проверяем, что все вершины достижимы
    EXPECT_EQ(result.edges.size(), 5);
}

// ============================================
// Тесты на ориентированных графах
// ============================================

TEST(DijkstraTest, DirectedSimple) {
    // 0 -> 1 -> 2
    auto g = createDirectedGraph(3, {
        {0, 1, 1},
        {1, 2, 2}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.dist.get(1), 1);
    EXPECT_EQ(result.dist.get(2), 3);
}

TEST(DijkstraTest, DirectedUnreachable) {
    // 0 -> 1    2 (изолирована)
    auto g = createDirectedGraph(3, {
        {0, 1, 1}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_FALSE(result.allReachable);
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.dist.get(1), 1);
    // dist[2] > INF_ALGO (недостижима)
}

TEST(DijkstraTest, DirectedCycle) {
    // 0 -> 1 -> 2 -> 0 (цикл)
    auto g = createDirectedGraph(3, {
        {0, 1, 1},
        {1, 2, 2},
        {2, 0, 3}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.dist.get(1), 1);
    EXPECT_EQ(result.dist.get(2), 3);
}

// ============================================
// Тесты на отрицательные веса
// ============================================

TEST(DijkstraTest, NegativeWeightThrows) {
    GraphMatrix g(3);
    g.addDirectedEdge(0, 1, -1);  // Отрицательный вес
    g.addDirectedEdge(1, 2, 2);
    
    EXPECT_THROW(Dijkstra::execute(g, 0), std::invalid_argument);
}

// ============================================
// Тесты восстановления пути
// ============================================

TEST(DijkstraTest, PathReconstruction) {
    // 0 --1-- 1 --2-- 2 --3-- 3
    auto g = createUndirectedGraph(4, {
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    // Путь до вершины 3
    auto path = result.getPath(3);
    
    EXPECT_EQ(path.size(), 4);
    EXPECT_EQ(path.get(0), 0);
    EXPECT_EQ(path.get(1), 1);
    EXPECT_EQ(path.get(2), 2);
    EXPECT_EQ(path.get(3), 3);
    
    // Проверяем корректность пути
    EXPECT_TRUE(isValidPath(g, path));
    EXPECT_EQ(computePathLength(g, path), 6);
}

TEST(DijkstraTest, PathToSource) {
    auto g = createUndirectedGraph(3, {
        {0, 1, 1},
        {1, 2, 2}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    // Путь до источника — только сам источник
    auto path = result.getPath(0);
    
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path.get(0), 0);
}

TEST(DijkstraTest, ShortestPathChoice) {
    /**
     *   0
     *  /|\
     * 1 | 10
     * | 5 |
     * 1   2
     *  \ /
     *   3
     *   2
     * 
     * От 0 до 2:
     * - Прямой путь: 0->2 = 10
     * - Через 1: 0->1->2 не существует
     * - Надо проверить выбор кратчайшего
     */
    auto g = createUndirectedGraph(4, {
        {0, 1, 1},
        {0, 2, 10},
        {0, 3, 5},
        {1, 3, 2},
        {2, 3, 2}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.dist.get(1), 1);
    EXPECT_EQ(result.dist.get(3), 3);  // min(5, 1+2) = 3
    EXPECT_EQ(result.dist.get(2), 5);  // min(10, 3+2) = 5
}

// ============================================
// Тесты на различные источники
// ============================================

TEST(DijkstraTest, DifferentSources) {
    auto g = createUndirectedGraph(4, {
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3}
    });
    
    // От вершины 0
    auto r0 = Dijkstra::execute(g, 0);
    EXPECT_EQ(r0.dist.get(3), 6);
    
    // От вершины 2
    auto r2 = Dijkstra::execute(g, 2);
    EXPECT_EQ(r2.dist.get(0), 3);
    EXPECT_EQ(r2.dist.get(3), 3);
}

// ============================================
// Тесты на несвязные графы
// ============================================

TEST(DijkstraTest, DisconnectedGraph) {
    // Два компонента: {0, 1} и {2, 3}
    auto g = createUndirectedGraph(4, {
        {0, 1, 1},
        {2, 3, 2}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_FALSE(result.allReachable);
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.dist.get(1), 1);
    // Вершины 2, 3 недостижимы
    
    // Дерево содержит только ребро 0-1
    EXPECT_EQ(result.edges.size(), 1);
}

// ============================================
// Тесты на большие графы
// ============================================

TEST(DijkstraTest, LargeLinearGraph) {
    const int n = 100;
    GraphMatrix g(n);
    
    for (int i = 0; i < n - 1; ++i) {
        g.addUndirectedEdge(i, i + 1, 1);
    }
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.dist.get(n - 1), n - 1);
    EXPECT_EQ(result.edges.size(), n - 1);
}

TEST(DijkstraTest, CompleteGraph) {
    // Полный граф на 5 вершинах
    const int n = 5;
    GraphMatrix g(n);
    
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            g.addUndirectedEdge(i, j, i + j + 1);
        }
    }
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.edges.size(), n - 1);
    
    // От 0 до каждой вершины прямое ребро — кратчайшее
    for (int i = 1; i < n; ++i) {
        EXPECT_EQ(result.dist.get(i), i + 1);  // Вес ребра 0-i = i+1
    }
}

// ============================================
// Тесты на нулевые веса
// ============================================

TEST(DijkstraTest, ZeroWeightEdges) {
    auto g = createUndirectedGraph(3, {
        {0, 1, 0},
        {1, 2, 0}
    });
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_TRUE(result.allReachable);
    EXPECT_EQ(result.dist.get(0), 0);
    EXPECT_EQ(result.dist.get(1), 0);
    EXPECT_EQ(result.dist.get(2), 0);
}

// ============================================
// Тесты на петли (self-loops)
// ============================================

TEST(DijkstraTest, SelfLoop) {
    GraphMatrix g(2);
    g.addDirectedEdge(0, 0, 5);  // Петля
    g.addDirectedEdge(0, 1, 1);
    
    auto result = Dijkstra::execute(g, 0);
    
    EXPECT_EQ(result.dist.get(0), 0);  // Петля не уменьшает расстояние
    EXPECT_EQ(result.dist.get(1), 1);
}