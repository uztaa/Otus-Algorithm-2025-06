#pragma once

#include "WeightedEdge.hpp"
#include "UnionFind.hpp"
#include "MSTResult.hpp"
#include "FactorArray.h"
#include "SpaceArray.h"
#include <limits>

/**
 * @brief Алгоритм Борувки для построения минимального остовного дерева
 * 
 * Исторически первый алгоритм MST (1926 год, Отакар Борувка).
 * 
 * Алгоритм работает следующим образом:
 * 1. Изначально каждая вершина — отдельная компонента (лес из V деревьев)
 * 2. На каждой итерации:
 *    - Для каждой компоненты находим минимальное ребро, ведущее наружу
 *    - Добавляем все найденные рёбра в MST
 *    - Объединяем соответствующие компоненты
 * 3. Повторяем, пока не останется одна компонента
 * 
 * Сложность:
 * - Время: O(E log V) — каждая итерация O(E), итераций O(log V)
 * - Память: O(V + E)
 * 
 * Преимущества:
 * - Хорошо параллелизуется (каждая компонента обрабатывается независимо)
 * - Количество итераций логарифмическое (каждая итерация уменьшает
 *   число компонент минимум вдвое)
 * 
 * @note Граф должен быть НЕОРИЕНТИРОВАННЫМ и ВЗВЕШЕННЫМ
 */
class BoruvkaMST {
public:
    /**
     * @brief Построить минимальное остовное дерево
     * 
     * @param adjList Взвешенный граф в виде списка смежности
     * @param vertexCount Количество вершин
     * @return MSTResult Рёбра MST и суммарный вес
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
        
        // Извлекаем все рёбра
        FactorArray<WeightedEdge> edges;
        extractEdges(adjList, vertexCount, edges);
        
        return executeInternal(edges, vertexCount);
    }
    
    /**
     * @brief Перегрузка для графа, заданного списком рёбер
     */
    static MSTResult execute(const FactorArray<WeightedEdge>& edges, int vertexCount) {
        return executeInternal(edges, vertexCount);
    }

private:
    static MSTResult executeInternal(const FactorArray<WeightedEdge>& edges, int vertexCount) {
        MSTResult result;
        
        if (vertexCount <= 1) {
            result.isConnected = true;
            return result;
        }
        
        const int INF = std::numeric_limits<int>::max();
        
        // Union-Find для отслеживания компонент
        UnionFind uf(vertexCount);
        
        int numComponents = vertexCount;
        int targetEdges = vertexCount - 1;
        
        // Итерации алгоритма Борувки
        // Максимум O(log V) итераций
        while (numComponents > 1 && result.edges.size() < targetEdges) {
            // cheapest[c] = индекс минимального ребра для компоненты c
            FactorArray<int> cheapest;
            for (int i = 0; i < vertexCount; ++i) {
                cheapest.add(-1, i);  // -1 означает "ребро не найдено"
            }
            
            // Для каждого ребра проверяем, соединяет ли оно разные компоненты
            for (int i = 0; i < edges.size(); ++i) {
                const WeightedEdge& e = edges.get(i);
                
                int comp1 = uf.find(e.from);
                int comp2 = uf.find(e.to);
                
                // Если ребро соединяет разные компоненты
                if (comp1 != comp2) {
                    // Обновляем cheapest для компоненты comp1
                    if (cheapest.get(comp1) == -1 || 
                        edges.get(cheapest.get(comp1)).weight > e.weight) {
                        setCheapest(cheapest, comp1, i);
                    }
                    
                    // Обновляем cheapest для компоненты comp2
                    if (cheapest.get(comp2) == -1 || 
                        edges.get(cheapest.get(comp2)).weight > e.weight) {
                        setCheapest(cheapest, comp2, i);
                    }
                }
            }
            
            // Добавляем найденные минимальные рёбра
            bool addedAny = false;
            for (int v = 0; v < vertexCount; ++v) {
                // Обрабатываем только корни компонент
                if (uf.find(v) != v) continue;
                
                int edgeIdx = cheapest.get(v);
                if (edgeIdx != -1) {
                    const WeightedEdge& e = edges.get(edgeIdx);
                    
                    // Проверяем ещё раз (могло измениться после предыдущих merge)
                    if (uf.merge(e.from, e.to)) {
                        result.edges.add(e.toEdge(), result.edges.size());
                        result.totalWeight += e.weight;
                        numComponents--;
                        addedAny = true;
                    }
                }
            }
            
            // Если не добавили ни одного ребра — граф несвязный
            if (!addedAny && numComponents > 1) {
                result.isConnected = false;
                return result;
            }
        }
        
        result.isConnected = (result.edges.size() == targetEdges);
        return result;
    }
    
    /**
     * @brief Извлечь рёбра из списка смежности
     */
    static void extractEdges(const SpaceArray<int>& adjList, int vertexCount,
                             FactorArray<WeightedEdge>& edges) {
        for (int v = 0; v < vertexCount; ++v) {
            if (v >= adjList.blockCount()) continue;
            
            int blockSize = adjList.blockSize(v);
            for (int i = 0; i < blockSize; i += 2) {
                int neighbor = adjList.get(v, i);
                int weight = adjList.get(v, i + 1);
                
                // Добавляем ребро только один раз
                if (v < neighbor) {
                    edges.add(WeightedEdge(v, neighbor, weight), edges.size());
                }
            }
        }
    }
    
    /**
     * @brief Вспомогательный метод для обновления FactorArray
     */
    static void setCheapest(FactorArray<int>& arr, int index, int value) {
        arr.remove(index);
        arr.add(value, index);
    }
};