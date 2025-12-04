#pragma once

#include "WeightedEdge.hpp"
#include "UnionFind.hpp"
#include "MSTResult.hpp"
#include "FactorArray.h"
#include "SpaceArray.h"

/**
 * @brief Алгоритм Краскала для построения минимального остовного дерева
 * 
 * Алгоритм работает следующим образом:
 * 1. Все рёбра графа сортируются по весу в порядке возрастания
 * 2. Проходим по отсортированным рёбрам и добавляем ребро в MST,
 *    если оно соединяет вершины из разных компонент (не образует цикл)
 * 3. Для проверки связности используется структура Union-Find
 * 4. Алгоритм завершается, когда добавлено V-1 ребро (дерево построено)
 * 
 * Сложность:
 * - Время: O(E log E) — определяется сортировкой рёбер
 * - Память: O(V + E) — для хранения Union-Find и списка рёбер
 * 
 * Преимущества:
 * - Хорошо работает на разреженных графах
 * - Простая реализация
 * - Легко параллелизуется (сортировка)
 * 
 * @note Граф должен быть НЕОРИЕНТИРОВАННЫМ и ВЗВЕШЕННЫМ
 */
class KruskalMST {
public:
    /**
     * @brief Построить минимальное остовное дерево
     * 
     * @param adjList Взвешенный граф в виде списка смежности.
     *                adjList содержит пары (сосед, вес):
     *                - adjList.get(v, 2*i) = номер соседа
     *                - adjList.get(v, 2*i+1) = вес ребра
     * @param vertexCount Количество вершин в графе
     * @return MSTResult Рёбра MST и суммарный вес
     * 
     * @note Вершины нумеруются с 0
     * @note Для неориентированного графа каждое ребро представлено дважды
     */
    static MSTResult execute(const SpaceArray<int>& adjList, int vertexCount) {
        MSTResult result;
        
        if (vertexCount <= 0) {
            result.isConnected = true;
            return result;
        }
        
        if (vertexCount == 1) {
            result.isConnected = true;
            return result;
        }
        
        // Шаг 1: Извлечь все рёбра из списка смежности
        FactorArray<WeightedEdge> allEdges;
        extractEdges(adjList, vertexCount, allEdges);
        
        // Шаг 2: Сортировка рёбер по весу
        sortEdges(allEdges);
        
        // Шаг 3: Инициализация Union-Find
        UnionFind uf(vertexCount);
        
        // Шаг 4: Жадный выбор рёбер
        int edgesAdded = 0;
        int targetEdges = vertexCount - 1;  // Дерево содержит V-1 ребро
        
        for (int i = 0; i < allEdges.size() && edgesAdded < targetEdges; ++i) {
            WeightedEdge edge = allEdges.get(i);
            
            int u = edge.from;
            int v = edge.to;
            
            // Если вершины в разных компонентах — добавляем ребро
            if (uf.merge(u, v)) {
                result.edges.add(edge.toEdge(), result.edges.size());
                result.totalWeight += edge.weight;
                edgesAdded++;
            }
        }
        
        // Проверяем, построено ли полное MST
        result.isConnected = (edgesAdded == targetEdges);
        
        return result;
    }
    
    /**
     * @brief Перегрузка для графа, заданного списком взвешенных рёбер
     * 
     * @param edges Массив взвешенных рёбер
     * @param vertexCount Количество вершин
     * @return MSTResult Результат
     */
    static MSTResult execute(const FactorArray<WeightedEdge>& edges, int vertexCount) {
        MSTResult result;
        
        if (vertexCount <= 0) {
            result.isConnected = true;
            return result;
        }
        
        if (vertexCount == 1) {
            result.isConnected = true;
            return result;
        }
        
        // Копируем рёбра для сортировки
        FactorArray<WeightedEdge> sortedEdges;
        for (int i = 0; i < edges.size(); ++i) {
            sortedEdges.add(edges.get(i), sortedEdges.size());
        }
        
        // Сортируем
        sortEdges(sortedEdges);
        
        // Union-Find
        UnionFind uf(vertexCount);
        
        int edgesAdded = 0;
        int targetEdges = vertexCount - 1;
        
        for (int i = 0; i < sortedEdges.size() && edgesAdded < targetEdges; ++i) {
            const WeightedEdge& edge = sortedEdges.get(i);
            
            if (uf.merge(edge.from, edge.to)) {
                result.edges.add(edge.toEdge(), result.edges.size());
                result.totalWeight += edge.weight;
                edgesAdded++;
            }
        }
        
        result.isConnected = (edgesAdded == targetEdges);
        return result;
    }

private:
    /**
     * @brief Извлечь рёбра из списка смежности
     * 
     * Формат списка смежности:
     * Для каждой вершины v данные хранятся парами (сосед, вес).
     * adjList.get(v, 2*i) = номер соседа
     * adjList.get(v, 2*i+1) = вес ребра до этого соседа
     * 
     * Поскольку граф неориентированный, ребро (u,v) присутствует
     * и в adjList[u], и в adjList[v]. Добавляем только если u < v,
     * чтобы избежать дублирования.
     */
    static void extractEdges(const SpaceArray<int>& adjList, int vertexCount,
                             FactorArray<WeightedEdge>& edges) {
        for (int v = 0; v < vertexCount; ++v) {
            if (v >= adjList.blockCount()) continue;
            
            int blockSize = adjList.blockSize(v);
            // Данные хранятся парами: (сосед, вес)
            for (int i = 0; i < blockSize; i += 2) {
                int neighbor = adjList.get(v, i);
                int weight = adjList.get(v, i + 1);
                
                // Добавляем ребро только один раз (когда v < neighbor)
                if (v < neighbor) {
                    edges.add(WeightedEdge(v, neighbor, weight), edges.size());
                }
            }
        }
    }
    
    /**
     * @brief Сортировка рёбер по весу (простая сортировка вставками)
     * 
     * Для небольших графов достаточно простой сортировки.
     * Для больших графов можно заменить на быструю сортировку.
     */
    static void sortEdges(FactorArray<WeightedEdge>& edges) {
        int n = edges.size();
        
        // Сортировка вставками — O(n²), но проста в реализации
        // Для учебных целей подходит, для production заменить на O(n log n)
        for (int i = 1; i < n; ++i) {
            WeightedEdge key = edges.get(i);
            int j = i - 1;
            
            // Сдвигаем элементы, большие key, вправо
            while (j >= 0 && edges.get(j).weight > key.weight) {
                // edges[j+1] = edges[j]
                WeightedEdge temp = edges.get(j);
                edges.remove(j + 1);
                edges.add(temp, j + 1);
                j--;
            }
            
            // Вставляем key на правильную позицию
            edges.remove(j + 1);
            edges.add(key, j + 1);
        }
    }
};