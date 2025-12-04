#pragma once

#include "WeightedEdge.hpp"
#include "FactorArray.h"
#include "SpaceArray.h"
#include <limits>
#include "MSTResult.hpp"

/**
 * @brief Алгоритм Прима для построения минимального остовного дерева
 * 
 * Алгоритм работает следующим образом:
 * 1. Начинаем с произвольной вершины (обычно 0)
 * 2. Поддерживаем множество вершин, уже включённых в дерево
 * 3. На каждом шаге добавляем ребро минимального веса,
 *    соединяющее вершину в дереве с вершиной вне дерева
 * 4. Повторяем, пока все вершины не будут включены
 * 
 * Сложность (данная реализация с массивом):
 * - Время: O(V²) — на каждом шаге ищем минимум среди всех вершин
 * - Память: O(V) — для хранения массивов key[] и parent[]
 * 
 * Оптимизации:
 * - С бинарной кучей: O(E log V)
 * - С кучей Фибоначчи: O(E + V log V)
 * 
 * Преимущества:
 * - Хорошо работает на плотных графах
 * - "Растит" дерево из одной точки — удобно для визуализации
 * 
 * @note Граф должен быть НЕОРИЕНТИРОВАННЫМ, ВЗВЕШЕННЫМ и СВЯЗНЫМ
 */
class PrimMST {
public:
    /**
     * @brief Построить минимальное остовное дерево
     * 
     * @param adjList Взвешенный граф в виде списка смежности.
     *                Формат: adjList.get(v, 2*i) = сосед, adjList.get(v, 2*i+1) = вес
     * @param vertexCount Количество вершин в графе
     * @return MSTResult Рёбра MST и суммарный вес
     * 
     * @note Вершины нумеруются с 0
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
        
        const int INF = std::numeric_limits<int>::max();
        
        // key[v] = минимальный вес ребра, соединяющего v с деревом
        FactorArray<int> key;
        // parent[v] = вершина в дереве, от которой идёт минимальное ребро к v
        FactorArray<int> parent;
        // inMST[v] = true, если вершина уже включена в дерево
        FactorArray<bool> inMST;
        
        // Инициализация
        for (int i = 0; i < vertexCount; ++i) {
            key.add(INF, i);
            parent.add(-1, i);
            inMST.add(false, i);
        }
        
        // Начинаем с вершины 0
        setKey(key, 0, 0);
        
        int edgesAdded = 0;
        
        // Основной цикл: добавляем V вершин (V-1 ребро)
        for (int count = 0; count < vertexCount; ++count) {
            // Находим вершину с минимальным key, не включённую в MST
            int u = -1;
            int minKey = INF;
            
            for (int v = 0; v < vertexCount; ++v) {
                if (!inMST.get(v) && key.get(v) < minKey) {
                    minKey = key.get(v);
                    u = v;
                }
            }
            
            // Если не нашли вершину — граф несвязный
            if (u == -1) {
                result.isConnected = false;
                return result;
            }
            
            // Включаем u в MST
            setInMST(inMST, u, true);
            
            // Добавляем ребро в результат (кроме начальной вершины)
            if (parent.get(u) != -1) {
                result.edges.add(Edge(parent.get(u), u), result.edges.size());
                result.totalWeight += key.get(u);
                edgesAdded++;
            }
            
            // Обновляем key[] для соседей u
            if (u < adjList.blockCount()) {
                int blockSize = adjList.blockSize(u);
                for (int i = 0; i < blockSize; i += 2) {
                    int v = adjList.get(u, i);
                    int weight = adjList.get(u, i + 1);
                    
                    // Если v не в MST и ребро (u,v) легче текущего лучшего
                    if (!inMST.get(v) && weight < key.get(v)) {
                        setKey(key, v, weight);
                        setParent(parent, v, u);
                    }
                }
            }
        }
        
        result.isConnected = (edgesAdded == vertexCount - 1);
        return result;
    }
    
    /**
     * @brief Перегрузка для графа, заданного списком рёбер
     */
    static MSTResult execute(const FactorArray<WeightedEdge>& edges, int vertexCount) {
        // Преобразуем в список смежности
        SpaceArray<int> adjList;
        
        for (int i = 0; i < edges.size(); ++i) {
            const WeightedEdge& e = edges.get(i);
            // Добавляем ребро в обе стороны
            adjList.add(e.from, e.to);
            adjList.add(e.from, e.weight);
            adjList.add(e.to, e.from);
            adjList.add(e.to, e.weight);
        }
        
        return execute(adjList, vertexCount);
    }

private:
    /**
     * @brief Вспомогательные методы для обновления FactorArray
     */
    static void setKey(FactorArray<int>& arr, int index, int value) {
        arr.remove(index);
        arr.add(value, index);
    }
    
    static void setParent(FactorArray<int>& arr, int index, int value) {
        arr.remove(index);
        arr.add(value, index);
    }
    
    static void setInMST(FactorArray<bool>& arr, int index, bool value) {
        arr.remove(index);
        arr.add(value, index);
    }
};