#include "DemukronAlgorithm.hpp"
#include <iostream>

/**
 * @brief Вспомогательная функция для печати результата
 */
void printResult(const DemukronResult& result) {
    if (result.hasCycle) {
        std::cout << "Граф содержит цикл! Топологическая сортировка невозможна.\n";
        return;
    }
    
    std::cout << "Топологическая сортировка по уровням:\n";
    for (int level = 0; level < result.levels.blockCount(); ++level) {
        std::cout << "  Уровень " << level << ": [";
        int size = result.levels.blockSize(level);
        for (int i = 0; i < size; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << result.levels.get(level, i);
        }
        std::cout << "]\n";
    }
}

/**
 * @brief Пример 1: Простой граф из документации
 * 
 *     0 ──→ 2 ──→ 4
 *     ↓     ↑
 *     1 ────┘
 * 
 * Рёбра: 0→1, 0→2, 1→2, 2→4
 * Ожидаемый результат:
 *   Уровень 0: [0, 3] (3 - изолированная вершина)
 *   Уровень 1: [1]
 *   Уровень 2: [2]
 *   Уровень 3: [4]
 */
void example1() {
    std::cout << "=== Пример 1: Простой DAG ===\n";
    
    SpaceArray<int> adjList;
    
    // Вершина 0 → 1, 2
    adjList.add(0, 1);
    adjList.add(0, 2);
    
    // Вершина 1 → 2
    adjList.add(1, 2);
    
    // Вершина 2 → 4
    adjList.add(2, 4);
    
    // Вершины 3 и 4 не имеют исходящих рёбер
    // (не нужно добавлять, SpaceArray обработает)
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 5);
    printResult(result);
    std::cout << "\n";
}

/**
 * @brief Пример 2: Граф с циклом
 * 
 *     0 ──→ 1 ──→ 2
 *           ↑     ↓
 *           └─────┘
 * 
 * Рёбра: 0→1, 1→2, 2→1 (цикл между 1 и 2)
 */
void example2() {
    std::cout << "=== Пример 2: Граф с циклом ===\n";
    
    SpaceArray<int> adjList;
    
    adjList.add(0, 1);
    adjList.add(1, 2);
    adjList.add(2, 1);  // Создаёт цикл
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 3);
    printResult(result);
    std::cout << "\n";
}

/**
 * @brief Пример 3: Граф из лекции (упрощённый)
 * 
 * Граф зависимостей сборки:
 *   Resources.obj (0), MessageSystem.obj (1) → KernelAPI.obj (2)
 *   KernelAPI.obj (2) → ExternalApi.obj (3) → Release.exe (4)
 *   DataCarriers.obj (5) → Cache.obj (6) → GUIAPI.obj (7) → Kernel.obj (8)
 *   Algorithms.obj (9) → NetSpan.obj (10) → Kernel.obj (8)
 *   Kernel.obj (8) → Release.exe (4)
 */
void example3() {
    std::cout << "=== Пример 3: Граф зависимостей сборки ===\n";
    
    SpaceArray<int> adjList;
    
    // Resources.obj (0) → ExternalApi.obj (3)
    adjList.add(0, 3);
    
    // MessageSystem.obj (1) → ExternalApi.obj (3)
    adjList.add(1, 3);
    
    // KernelAPI.obj (2) → ExternalApi.obj (3)
    adjList.add(2, 3);
    
    // ExternalApi.obj (3) → Release.exe (4)
    adjList.add(3, 4);
    
    // DataCarriers.obj (5) → Cache.obj (6)
    adjList.add(5, 6);
    
    // Cache.obj (6) → GUIAPI.obj (7)
    adjList.add(6, 7);
    
    // GUIAPI.obj (7) → Kernel.obj (8)
    adjList.add(7, 8);
    
    // Algorithms.obj (9) → NetSpan.obj (10)
    adjList.add(9, 10);
    
    // NetSpan.obj (10) → Kernel.obj (8)
    adjList.add(10, 8);
    
    // Kernel.obj (8) → Release.exe (4)
    adjList.add(8, 4);
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 11);
    printResult(result);
    
    std::cout << "\nИнтерпретация: вершины одного уровня можно компилировать параллельно.\n";
    std::cout << "\n";
}

/**
 * @brief Пример 4: Пустой граф
 */
void example4() {
    std::cout << "=== Пример 4: Пустой граф ===\n";
    
    SpaceArray<int> adjList;
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 0);
    printResult(result);
    std::cout << "\n";
}

/**
 * @brief Пример 5: Граф без рёбер (все вершины изолированы)
 */
void example5() {
    std::cout << "=== Пример 5: Изолированные вершины ===\n";
    
    SpaceArray<int> adjList;
    // Не добавляем рёбра - все 4 вершины изолированы
    
    DemukronResult result = DemukronAlgorithm::execute(adjList, 4);
    printResult(result);
    std::cout << "\n";
}

int main() {
    std::cout << "Демонстрация алгоритма Демукрона\n";
    std::cout << "================================\n\n";
    
    example1();
    example2();
    example3();
    example4();
    example5();
    
    return 0;
}