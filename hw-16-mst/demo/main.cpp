#include "KruskalMST.hpp"
#include "PrimMST.hpp"
#include "BoruvkaMST.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

/**
 * @brief Вспомогательная функция для печати результата MST
 */
void printMSTResult(const std::string& algorithmName, const MSTResult& result) {
    std::cout << "=== " << algorithmName << " ===\n";
    
    if (!result.isConnected) {
        std::cout << "Граф несвязный! MST не может быть построено полностью.\n";
    }
    
    std::cout << "Рёбра MST:\n";
    for (int i = 0; i < result.edges.size(); ++i) {
        Edge e = result.edges.get(i);
        std::cout << "  " << e.v1 << " -- " << e.v2 << "\n";
    }
    
    std::cout << "Суммарный вес: " << result.totalWeight << "\n";
    std::cout << "Количество рёбер: " << result.edges.size() << "\n\n";
}

/**
 * @brief Создание взвешенного графа
 */
SpaceArray<int> createGraph(const std::vector<std::tuple<int, int, int>>& edges) {
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
 * @brief Пример 1: Граф из лекции
 * 
 *        A(0)
 *       / | 
 *      7  5  
 *     /   |   
 *   B(1)--D(3)
 *   |\ 9  | \
 *   | \   |  6
 *  8|  7 15   \
 *   |   \ |    F(5)
 *   C(2)-E(4)  /|
 *     5   \  8/ |
 *          \/  11
 *          G(6)
 */
void example1() {
    std::cout << "========================================\n";
    std::cout << "Пример 1: Граф из лекции (7 вершин)\n";
    std::cout << "========================================\n\n";
    
    auto adjList = createGraph({
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
    });
    
    int vertexCount = 7;
    
    // Алгоритм Краскала
    MSTResult kruskalResult = KruskalMST::execute(adjList, vertexCount);
    printMSTResult("Алгоритм Краскала", kruskalResult);
    
    // Алгоритм Прима
    MSTResult primResult = PrimMST::execute(adjList, vertexCount);
    printMSTResult("Алгоритм Прима", primResult);
    
    // Алгоритм Борувки
    MSTResult boruvkaResult = BoruvkaMST::execute(adjList, vertexCount);
    printMSTResult("Алгоритм Борувки", boruvkaResult);
    
    std::cout << "Ожидаемый вес MST: 39 (5+5+6+7+7+9)\n\n";
}

/**
 * @brief Пример 2: Простой треугольник
 */
void example2() {
    std::cout << "========================================\n";
    std::cout << "Пример 2: Треугольник\n";
    std::cout << "========================================\n\n";
    
    std::cout << "    0\n";
    std::cout << "   /|\\\n";
    std::cout << "  1 | 3\n";
    std::cout << " /  |  \\\n";
    std::cout << "1---2---2\n";
    std::cout << "    2\n\n";
    
    auto adjList = createGraph({
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 3}
    });
    
    MSTResult result = KruskalMST::execute(adjList, 3);
    printMSTResult("Алгоритм Краскала", result);
    
    std::cout << "Ожидаемый вес MST: 3 (рёбра 0-1 и 1-2)\n\n";
}

/**
 * @brief Пример 3: Несвязный граф
 */
void example3() {
    std::cout << "========================================\n";
    std::cout << "Пример 3: Несвязный граф\n";
    std::cout << "========================================\n\n";
    
    std::cout << "0 -- 1    2 -- 3\n";
    std::cout << "  1         2\n\n";
    
    auto adjList = createGraph({
        {0, 1, 1},
        {2, 3, 2}
    });
    
    MSTResult result = KruskalMST::execute(adjList, 4);
    printMSTResult("Алгоритм Краскала", result);
    
    std::cout << "Граф несвязный — полное MST невозможно.\n\n";
}

/**
 * @brief Пример 4: Звезда
 */
void example4() {
    std::cout << "========================================\n";
    std::cout << "Пример 4: Звезда (5 вершин)\n";
    std::cout << "========================================\n\n";
    
    std::cout << "    1\n";
    std::cout << "    |1\n";
    std::cout << "4-4-0-2-2\n";
    std::cout << "    |3\n";
    std::cout << "    3\n\n";
    
    auto adjList = createGraph({
        {0, 1, 1},
        {0, 2, 2},
        {0, 3, 3},
        {0, 4, 4}
    });
    
    MSTResult result = KruskalMST::execute(adjList, 5);
    printMSTResult("Алгоритм Краскала", result);
    
    std::cout << "Ожидаемый вес MST: 10 (1+2+3+4)\n\n";
}

/**
 * @brief Сравнение производительности алгоритмов
 */
void performanceComparison() {
    std::cout << "========================================\n";
    std::cout << "Сравнение алгоритмов\n";
    std::cout << "========================================\n\n";
    
    std::cout << "| Алгоритм | Сложность     | Лучший случай           |\n";
    std::cout << "|----------|---------------|-------------------------|\n";
    std::cout << "| Краскал  | O(E log E)    | Разреженные графы       |\n";
    std::cout << "| Прим     | O(V²) / O(E log V) | Плотные графы      |\n";
    std::cout << "| Борувка  | O(E log V)    | Параллельные вычисления |\n\n";
}

int main() {
    std::cout << "Демонстрация алгоритмов минимального остовного дерева\n";
    std::cout << "====================================================\n\n";
    
    example1();
    example2();
    example3();
    example4();
    performanceComparison();
    
    return 0;
}