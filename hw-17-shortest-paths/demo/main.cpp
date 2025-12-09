#include "Dijkstra.hpp"
#include "FloydWarshall.hpp"
#include "GraphMatrix.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

/**
 * @brief Вспомогательная функция для создания неориентированного графа
 */
GraphMatrix createGraph(int n, const std::vector<std::tuple<int, int, int>>& edges) {
    GraphMatrix g(n);
    for (const auto& [u, v, w] : edges) {
        g.addUndirectedEdge(u, v, w);
    }
    return g;
}

/**
 * @brief Вспомогательная функция для печати результата Дейкстры
 */
void printDijkstraResult(const std::string& title, 
                         const GraphMatrix& g,
                         const ShortestPathResult& result) {
    std::cout << "=== " << title << " ===\n";
    std::cout << "Источник: вершина " << result.sourceVertex << "\n\n";
    
    // Расстояния
    std::cout << "Расстояния от источника:\n";
    int n = result.dist.size();
    for (int i = 0; i < n; ++i) {
        std::cout << "  " << result.sourceVertex << " -> " << i << ": ";
        // Недостижима, если нет родителя и это не источник
        if (result.parent.get(i) == -1 && i != result.sourceVertex) {
            std::cout << "недостижима";
        } else {
            std::cout << result.dist.get(i);
        }
        std::cout << "\n";
    }
    
    // Рёбра дерева кратчайших путей
    std::cout << "\nДерево кратчайших путей (рёбра):\n";
    for (int i = 0; i < result.edges.size(); ++i) {
        Edge e = result.edges.get(i);
        int w = g.getWeight(e.v1, e.v2);
        std::cout << "  " << e.v1 << " -- " << e.v2 << " (вес " << w << ")\n";
    }
    
    std::cout << "\nВсе вершины достижимы: " 
              << (result.allReachable ? "Да" : "Нет") << "\n";
    std::cout << "\n";
}

/**
 * @brief Печать пути до конкретной вершины
 */
void printPath(const ShortestPathResult& result, int target) {
    auto path = result.getPath(target);
    
    std::cout << "Путь до вершины " << target << ": ";
    for (int i = 0; i < path.size(); ++i) {
        std::cout << path.get(i);
        if (i < path.size() - 1) std::cout << " -> ";
    }
    std::cout << " (длина: " << result.dist.get(target) << ")\n";
}

// ============================================
// Примеры
// ============================================

/**
 * @brief Пример 1: Граф из лекции
 * 
 *        7         9
 *  [0] ----- [1] ----- [2]
 *   |         |\        |
 *   14       10  2      11
 *   |         |    \    |
 *  [3] ----- [4] ----- [5]
 *        9         6
 */
void example1() {
    std::cout << "========================================\n";
    std::cout << "Пример 1: Граф из лекции (6 вершин)\n";
    std::cout << "========================================\n\n";
    
    auto g = createGraph(6, {
        {0, 1, 7},   // A-B
        {0, 3, 14},  // A-D
        {1, 2, 9},   // B-C
        {1, 3, 10},  // B-D
        {1, 4, 2},   // B-E
        {2, 4, 11},  // C-E
        {2, 5, 6},   // C-F
        {3, 4, 9},   // D-E
        {4, 5, 6}    // E-F
    });
    
    std::cout << "Структура графа:\n";
    std::cout << "       7         9\n";
    std::cout << " [0] ----- [1] ----- [2]\n";
    std::cout << "  |         |\\        |\n";
    std::cout << "  14       10  2      11\n";
    std::cout << "  |         |    \\    |\n";
    std::cout << " [3] ----- [4] ----- [5]\n";
    std::cout << "       9         6\n\n";
    
    g.print(std::cout, "Матрица смежности");
    std::cout << "\n";
    
    auto result = Dijkstra::execute(g, 0);  // От вершины 0
    printDijkstraResult("Алгоритм Дейкстры от вершины 0", g, result);
    
    // Пути до каждой вершины
    std::cout << "Восстановленные пути:\n";
    for (int i = 0; i < 6; ++i) {
        printPath(result, i);
    }
    std::cout << "\n";
}

/**
 * @brief Пример 2: Простой треугольник
 */
void example2() {
    std::cout << "========================================\n";
    std::cout << "Пример 2: Треугольник\n";
    std::cout << "========================================\n\n";
    
    std::cout << "      [0]\n";
    std::cout << "      / \\\n";
    std::cout << "     1   3\n";
    std::cout << "    /     \\\n";
    std::cout << " [1] --2-- [2]\n\n";
    
    auto g = createGraph(3, {
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 3}
    });
    
    auto result = Dijkstra::execute(g, 0);
    printDijkstraResult("Алгоритм Дейкстры от вершины 0", g, result);
    
    std::cout << "Замечание: до вершины 2 можно дойти двумя путями:\n";
    std::cout << "  - напрямую 0->2 = 3\n";
    std::cout << "  - через 1: 0->1->2 = 1+2 = 3\n";
    std::cout << "Оба пути одинаковой длины.\n\n";
}

/**
 * @brief Пример 3: Линейный граф
 */
void example3() {
    std::cout << "========================================\n";
    std::cout << "Пример 3: Линейный граф\n";
    std::cout << "========================================\n\n";
    
    std::cout << "[0] --1-- [1] --2-- [2] --3-- [3] --4-- [4]\n\n";
    
    auto g = createGraph(5, {
        {0, 1, 1},
        {1, 2, 2},
        {2, 3, 3},
        {3, 4, 4}
    });
    
    auto result = Dijkstra::execute(g, 0);
    printDijkstraResult("Алгоритм Дейкстры от вершины 0", g, result);
}

/**
 * @brief Пример 4: Несвязный граф
 */
void example4() {
    std::cout << "========================================\n";
    std::cout << "Пример 4: Несвязный граф\n";
    std::cout << "========================================\n\n";
    
    std::cout << "[0] --1-- [1]    [2] --2-- [3]\n\n";
    
    auto g = createGraph(4, {
        {0, 1, 1},
        {2, 3, 2}
    });
    
    auto result = Dijkstra::execute(g, 0);
    printDijkstraResult("Алгоритм Дейкстры от вершины 0", g, result);
    
    std::cout << "Вершины 2 и 3 недостижимы из вершины 0.\n\n";
}

/**
 * @brief Пример 5: Граф звезда
 */
void example5() {
    std::cout << "========================================\n";
    std::cout << "Пример 5: Звезда (5 вершин)\n";
    std::cout << "========================================\n\n";
    
    std::cout << "        [1]\n";
    std::cout << "         |\n";
    std::cout << "         1\n";
    std::cout << "         |\n";
    std::cout << "[4]--4--[0]--2--[2]\n";
    std::cout << "         |\n";
    std::cout << "         3\n";
    std::cout << "         |\n";
    std::cout << "        [3]\n\n";
    
    auto g = createGraph(5, {
        {0, 1, 1},
        {0, 2, 2},
        {0, 3, 3},
        {0, 4, 4}
    });
    
    auto result = Dijkstra::execute(g, 0);
    printDijkstraResult("Алгоритм Дейкстры от центра (0)", g, result);
    
    // Теперь от периферийной вершины
    auto result2 = Dijkstra::execute(g, 1);
    printDijkstraResult("Алгоритм Дейкстры от вершины 1", g, result2);
}

/**
 * @brief Пример 6: Выбор кратчайшего пути
 */
void example6() {
    std::cout << "========================================\n";
    std::cout << "Пример 6: Выбор кратчайшего пути\n";
    std::cout << "========================================\n\n";
    
    std::cout << "         [0]\n";
    std::cout << "        / | \\\n";
    std::cout << "       1  5  10\n";
    std::cout << "      /   |    \\\n";
    std::cout << "   [1]---[3]---[2]\n";
    std::cout << "       2     2\n\n";
    
    auto g = createGraph(4, {
        {0, 1, 1},
        {0, 3, 5},
        {0, 2, 10},
        {1, 3, 2},
        {3, 2, 2}
    });
    
    auto result = Dijkstra::execute(g, 0);
    printDijkstraResult("Алгоритм Дейкстры", g, result);
    
    std::cout << "До вершины 2 можно дойти:\n";
    std::cout << "  - напрямую: 0->2 = 10\n";
    std::cout << "  - через 3: 0->3->2 = 5+2 = 7\n";
    std::cout << "  - через 1,3: 0->1->3->2 = 1+2+2 = 5 (кратчайший!)\n";
    printPath(result, 2);
    std::cout << "\n";
}

/**
 * @brief Пример 7: Алгоритм Флойда-Уоршелла
 */
void example7() {
    std::cout << "========================================\n";
    std::cout << "Пример 7: Алгоритм Флойда-Уоршелла\n";
    std::cout << "========================================\n\n";
    
    std::cout << "Граф с 4 вершинами:\n";
    std::cout << "       1\n";
    std::cout << "[0] -----> [1]\n";
    std::cout << " |          |\n";
    std::cout << " |5         |2\n";
    std::cout << " v          v\n";
    std::cout << "[3] <----- [2]\n";
    std::cout << "       3\n\n";
    
    GraphMatrix g(4);
    g.addDirectedEdge(0, 1, 1);
    g.addDirectedEdge(0, 3, 5);
    g.addDirectedEdge(1, 2, 2);
    g.addDirectedEdge(2, 3, 3);
    
    auto result = FloydWarshall::execute(g);
    
    std::cout << "=== Результат Флойда-Уоршелла ===\n\n";
    
    std::cout << "Матрица кратчайших расстояний:\n";
    std::cout << "     ";
    for (int j = 0; j < 4; ++j) std::cout << "  [" << j << "]";
    std::cout << "\n";
    
    for (int i = 0; i < 4; ++i) {
        std::cout << "[" << i << "]: ";
        for (int j = 0; j < 4; ++j) {
            int d = result.getDistance(i, j);
            if (d == AllPairsShortestPathResult::INF) {
                std::cout << "  INF";
            } else {
                std::cout << "    " << d;
            }
        }
        std::cout << "\n";
    }
    
    std::cout << "\nПримеры путей:\n";
    
    // Путь 0 -> 3
    auto path03 = result.getPath(0, 3);
    std::cout << "  Путь [0] -> [3]: ";
    for (int i = 0; i < path03.size(); ++i) {
        std::cout << "[" << path03.get(i) << "]";
        if (i < path03.size() - 1) std::cout << " -> ";
    }
    std::cout << " (длина: " << result.getDistance(0, 3) << ")\n";
    
    // Путь 0 -> 2
    auto path02 = result.getPath(0, 2);
    std::cout << "  Путь [0] -> [2]: ";
    for (int i = 0; i < path02.size(); ++i) {
        std::cout << "[" << path02.get(i) << "]";
        if (i < path02.size() - 1) std::cout << " -> ";
    }
    std::cout << " (длина: " << result.getDistance(0, 2) << ")\n";
    
    std::cout << "\nОбратите внимание:\n";
    std::cout << "  - Путь [0] -> [3] напрямую = 5\n";
    std::cout << "  - Путь [0] -> [1] -> [2] -> [3] = 1+2+3 = 6 > 5\n";
    std::cout << "  - Поэтому выбран прямой путь!\n\n";
    
    std::cout << "Отрицательный цикл: " 
              << (result.hasNegativeCycle ? "Обнаружен" : "Нет") << "\n\n";
}

/**
 * @brief Пример 8: Флойд-Уоршелл с отрицательными весами
 */
void example8() {
    std::cout << "========================================\n";
    std::cout << "Пример 8: Отрицательные веса\n";
    std::cout << "========================================\n\n";
    
    std::cout << "Граф с отрицательным ребром:\n";
    std::cout << "       4\n";
    std::cout << "[0] -----> [1]\n";
    std::cout << " |          |\n";
    std::cout << " |5        -3\n";
    std::cout << " v          v\n";
    std::cout << "[2] <------+\n\n";
    
    GraphMatrix g(3);
    g.addDirectedEdge(0, 1, 4);
    g.addDirectedEdge(0, 2, 5);
    g.addDirectedEdge(1, 2, -3);  // Отрицательное ребро!
    
    auto result = FloydWarshall::execute(g);
    
    std::cout << "=== Результат ===\n\n";
    std::cout << "Расстояние [0] -> [2]:\n";
    std::cout << "  - Напрямую: 5\n";
    std::cout << "  - Через [1]: 4 + (-3) = 1 (короче!)\n";
    std::cout << "  - Результат: " << result.getDistance(0, 2) << "\n\n";
    
    std::cout << "Дейкстра НЕ работает с отрицательными весами,\n";
    std::cout << "а Флойд-Уоршелл — работает!\n\n";
    
    std::cout << "Отрицательный цикл: " 
              << (result.hasNegativeCycle ? "Обнаружен" : "Нет") << "\n\n";
}

/**
 * @brief Сравнение сложностей алгоритмов
 */
void showComplexity() {
    std::cout << "========================================\n";
    std::cout << "Сравнение алгоритмов кратчайших путей\n";
    std::cout << "========================================\n\n";
    
    std::cout << "| Алгоритм        | Сложность      | Отриц. веса | Все пары |\n";
    std::cout << "|-----------------|----------------|-------------|----------|\n";
    std::cout << "| Дейкстра        | O(V² log V)*   | Нет         | Нет      |\n";
    std::cout << "| Беллман-Форд    | O(V × E)       | Да          | Нет      |\n";
    std::cout << "| Флойд-Уоршелл   | O(V³)          | Да          | Да       |\n";
    std::cout << "| Джонсон         | O(V × E log V) | Да          | Да       |\n\n";
    
    std::cout << "* При использовании матрицы смежности и бинарной кучи\n";
    std::cout << "  Для списка смежности: O((V + E) log V)\n\n";
}

int main() {
    std::cout << "Демонстрация алгоритма Дейкстры\n";
    std::cout << "================================\n\n";
    
    example1();
    example2();
    example3();
    example4();
    example5();
    example6();
    example7();
    example8();
    showComplexity();
    
    return 0;
}