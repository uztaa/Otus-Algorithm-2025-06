#pragma once

#include "SpaceArray.h"
#include "FactorArray.h"

/**
 * @brief Ребро графа
 */
struct Edge {
    int from;  ///< Начальная вершина
    int to;    ///< Конечная вершина
    
    bool operator==(const Edge& other) const {
        return from == other.from && to == other.to;
    }
};

/**
 * @brief Результат поиска мостов и точек сочленения
 * 
 * Мост — ребро, удаление которого увеличивает число компонент связности.
 * Точка сочленения — вершина, удаление которой увеличивает число компонент связности.
 */
struct BridgesAPResult {
    FactorArray<Edge> bridges;              ///< Список мостов
    FactorArray<int> articulationPoints;    ///< Список точек сочленения
};

/**
 * @brief Алгоритм поиска мостов и точек сочленения в неориентированном графе
 * 
 * Оба поиска основаны на DFS и используют концепции disc[] и low[]:
 * 
 * 1. disc[v] (discovery time) — время обнаружения вершины при DFS.
 * 
 * 2. low[v] (low-link value) — минимальное время обнаружения среди:
 *    - самой вершины v
 *    - всех вершин в поддереве v
 *    - вершин, достижимых через одно обратное ребро из поддерева v
 * 
 * МОСТ (u, v), где u — родитель v в DFS-дереве:
 *   Ребро является мостом, если low[v] > disc[u].
 *   Интуиция: из поддерева v нет "обходного пути" обратно к u или выше,
 *   поэтому удаление ребра (u, v) разорвёт связность.
 * 
 * ТОЧКА СОЧЛЕНЕНИЯ u:
 *   Случай 1: u — корень DFS-дерева с >= 2 детьми.
 *             Удаление корня разобщит его поддеревья.
 *   Случай 2: u — не корень, и существует ребёнок v с low[v] >= disc[u].
 *             Из поддерева v нельзя подняться выше u, поэтому удаление u
 *             отрежет поддерево v от остального графа.
 * 
 * Сложность: O(V + E)
 * 
 * @note Граф должен быть НЕОРИЕНТИРОВАННЫМ. При хранении в списке смежности
 *       каждое ребро представлено дважды: (u, v) и (v, u).
 */
class BridgesAndAP {
public:
    /**
     * @brief Найти все мосты и точки сочленения
     * 
     * @param adjList Граф в виде списка смежности (SpaceArray)
     *                Для неориентированного графа: если есть ребро (u, v),
     *                то v есть в adjList[u] и u есть в adjList[v]
     * @param vertexCount Количество вершин в графе
     * @return BridgesAPResult Списки мостов и точек сочленения
     * 
     * @note Вершины нумеруются с 0
     */
    static BridgesAPResult execute(const SpaceArray<int>& adjList, int vertexCount) {
        BridgesAPResult result;
        
        if (vertexCount <= 0) {
            return result;
        }
        
        // Инициализация
        FactorArray<int> disc;       // Время обнаружения
        FactorArray<int> low;        // Low-link value
        FactorArray<int> parent;     // Родитель в DFS-дереве (-1 для корня)
        FactorArray<bool> visited;   // Посещена ли вершина
        FactorArray<bool> isAP;      // Является ли точкой сочленения (для дедупликации)
        
        for (int i = 0; i < vertexCount; ++i) {
            disc.add(0, disc.size());
            low.add(0, low.size());
            parent.add(-1, parent.size());
            visited.add(false, visited.size());
            isAP.add(false, isAP.size());
        }
        
        int timer = 0;
        
        // Запускаем DFS из каждой непосещённой вершины
        for (int v = 0; v < vertexCount; ++v) {
            if (!visited.get(v)) {
                dfs(v, adjList, disc, low, parent, visited, isAP, timer, result);
            }
        }
        
        // Собираем точки сочленения в результат
        for (int v = 0; v < vertexCount; ++v) {
            if (isAP.get(v)) {
                result.articulationPoints.add(v, result.articulationPoints.size());
            }
        }
        
        return result;
    }

private:
    /**
     * @brief Рекурсивный DFS для поиска мостов и точек сочленения
     */
    static void dfs(int u,
                    const SpaceArray<int>& adjList,
                    FactorArray<int>& disc,
                    FactorArray<int>& low,
                    FactorArray<int>& parent,
                    FactorArray<bool>& visited,
                    FactorArray<bool>& isAP,
                    int& timer,
                    BridgesAPResult& result) {
        
        // Помечаем вершину как посещённую
        visited.remove(u);
        visited.add(true, u);
        
        // Устанавливаем время обнаружения и начальное значение low
        disc.remove(u);
        disc.add(timer, u);
        low.remove(u);
        low.add(timer, u);
        timer++;
        
        int childCount = 0;  // Количество детей в DFS-дереве (для проверки корня)
        
        // Обходим всех соседей (если у вершины есть рёбра)
        if (u < adjList.blockCount()) {
            int neighborCount = adjList.blockSize(u);
            for (int i = 0; i < neighborCount; ++i) {
                int v = adjList.get(u, i);
                
                if (!visited.get(v)) {
                    // v ещё не посещена — это ребро дерева
                    childCount++;
                    
                    // Устанавливаем родителя
                    parent.remove(v);
                    parent.add(u, v);
                    
                    // Рекурсивный вызов
                    dfs(v, adjList, disc, low, parent, visited, isAP, timer, result);
                    
                    // Обновляем low[u] после возврата
                    int newLow = std::min(low.get(u), low.get(v));
                    low.remove(u);
                    low.add(newLow, u);
                    
                    // Проверка на мост: low[v] > disc[u]
                    // Из v нельзя достичь u или его предков иначе как через ребро (u, v)
                    if (low.get(v) > disc.get(u)) {
                        Edge bridge;
                        bridge.from = u;
                        bridge.to = v;
                        result.bridges.add(bridge, result.bridges.size());
                    }
                    
                    // Проверка на точку сочленения (случай 2):
                    // u не корень и low[v] >= disc[u]
                    if (parent.get(u) != -1 && low.get(v) >= disc.get(u)) {
                        isAP.remove(u);
                        isAP.add(true, u);
                    }
                }
                else if (v != parent.get(u)) {
                    // v уже посещена и это не родитель — обратное ребро
                    // Обновляем low[u] через это обратное ребро
                    int newLow = std::min(low.get(u), disc.get(v));
                    low.remove(u);
                    low.add(newLow, u);
                }
                // Если v == parent[u], это просто ребро обратно к родителю,
                // его игнорируем (мы пришли оттуда)
            }
        }
        
        // Проверка на точку сочленения (случай 1):
        // u — корень DFS-дерева с >= 2 детьми
        if (parent.get(u) == -1 && childCount >= 2) {
            isAP.remove(u);
            isAP.add(true, u);
        }
    }
};