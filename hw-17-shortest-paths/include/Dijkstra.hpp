#pragma once

#include "GraphMatrix.hpp"
#include "ShortestPathResult.hpp"
#include "PriorityQueueArrayList.h"
#include "FactorArray.h"
#include <stdexcept>

/**
 * @brief Алгоритм Дейкстры для поиска кратчайших путей
 * 
 * Находит кратчайшие пути от заданной вершины-источника до всех
 * остальных вершин графа. Работает только с неотрицательными весами.
 * 
 * Сложность:
 * - Время: O(V² log V) при использовании матрицы смежности
 * - Память: O(V)
 * 
 * @note Использует PriorityQueueArrayList из arrayslib для приоритетной очереди.
 */
class Dijkstra {
public:
    /**
     * @brief Выполнить алгоритм Дейкстры
     * 
     * @param graph Граф в виде матрицы смежности
     * @param source Вершина-источник
     * @return ShortestPathResult Дерево кратчайших путей и расстояния
     * @throws std::invalid_argument Если граф содержит отрицательные веса
     * @throws std::out_of_range Если source вне диапазона вершин
     */
    static ShortestPathResult execute(const GraphMatrix& graph, int source) {
        int n = graph.size();
        
        // Валидация
        if (n == 0 || source < 0 || source >= n) {
            throw std::out_of_range("source vertex out of range");
        }
        validateNoNegativeWeights(graph);
        
        ShortestPathResult result;
        result.sourceVertex = source;
        
        // Одновершинный граф
        if (n == 1) {
            result.dist.add(0, 0);
            result.parent.add(-1, 0);
            result.allReachable = true;
            return result;
        }
        
        // Инициализация
        int maxPriority = computeMaxPriority(graph);
        int INF_ALGO = maxPriority + 1;
        
        // Массивы расстояний и родителей
        for (int i = 0; i < n; ++i) {
            result.dist.add(INF_ALGO, i);
            result.parent.add(-1, i);
        }
        
        // Расстояние до источника = 0
        setArrayValue(result.dist, source, 0);
        
        // Массив посещённых вершин (0 = не посещена, 1 = посещена)
        FactorArray<int> visited;
        for (int i = 0; i < n; ++i) {
            visited.add(0, i);
        }
        
        // Приоритетная очередь: (приоритет = расстояние, элемент = вершина)
        PriorityQueueArrayList<int> pq(0, maxPriority);
        pq.enqueue(0, source);
        
        while (!pq.empty()) {
            int u = pq.dequeue();
            
            // Пропускаем уже обработанные вершины
            if (visited.get(u) != 0) {
                continue;
            }
            setArrayValue(visited, u, 1);
            
            int du = result.dist.get(u);
            
            // Релаксация всех соседей
            for (int v = 0; v < n; ++v) {
                if (visited.get(v) != 0) continue;
                
                int w = graph.getWeight(u, v);
                if (w == GraphMatrix::INF) continue;  // Нет ребра
                
                int newDist = du + w;
                if (newDist < result.dist.get(v)) {
                    setArrayValue(result.dist, v, newDist);
                    setArrayValue(result.parent, v, u);
                    pq.enqueue(newDist, v);
                }
            }
        }
        
        // Построение дерева рёбер из массива parent
        buildEdgesTree(result, n, INF_ALGO);
        
        return result;
    }

private:
    /**
     * @brief Вычисляет максимальный приоритет для очереди
     * (сумма всех положительных весов рёбер)
     */
    static int computeMaxPriority(const GraphMatrix& graph) {
        int sum = 0;
        int n = graph.size();
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int w = graph.getWeight(i, j);
                if (w != GraphMatrix::INF && w > 0) {
                    sum += w;
                }
            }
        }
        return (sum == 0) ? 1 : sum;
    }
    
    /**
     * @brief Проверяет отсутствие отрицательных весов
     * @throws std::invalid_argument Если найден отрицательный вес
     */
    static void validateNoNegativeWeights(const GraphMatrix& graph) {
        int n = graph.size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int w = graph.getWeight(i, j);
                if (w != GraphMatrix::INF && w < 0) {
                    throw std::invalid_argument(
                        "Dijkstra: graph contains negative edge weight"
                    );
                }
            }
        }
    }
    
    /**
     * @brief Установить значение в FactorArray по индексу
     * 
     * Поскольку FactorArray не имеет метода set(), используем remove + add.
     * Это работает корректно для одномерных массивов (не вложенных).
     */
    static void setArrayValue(FactorArray<int>& arr, int index, int value) {
        arr.remove(index);
        arr.add(value, index);
    }
    
    /**
     * @brief Построить массив рёбер дерева кратчайших путей
     */
    static void buildEdgesTree(ShortestPathResult& result, int n, int INF_ALGO) {
        result.allReachable = true;
        
        for (int v = 0; v < n; ++v) {
            int p = result.parent.get(v);
            if (p != -1) {
                // Ребро от родителя к вершине v
                result.edges.add(Edge(p, v), result.edges.size());
            } else if (v != result.sourceVertex) {
                // Вершина недостижима (нет родителя и это не источник)
                if (result.dist.get(v) >= INF_ALGO) {
                    result.allReachable = false;
                }
            }
        }
    }
};