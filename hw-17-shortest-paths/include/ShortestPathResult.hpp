#pragma once

#include "Edge.hpp"
#include "FactorArray.h"

/**
 * @brief Результат работы алгоритмов поиска кратчайших путей
 * 
 * Содержит дерево кратчайших путей от источника до всех достижимых вершин,
 * а также вспомогательную информацию (расстояния, родители).
 * 
 * @note Структура аналогична MSTResult из ДЗ-16.
 *       В будущем рекомендуется вынести в общую библиотеку graphlib.
 */
struct ShortestPathResult {
    FactorArray<Edge> edges;    ///< Рёбра дерева кратчайших путей
    FactorArray<int> dist;      ///< Расстояния от источника до каждой вершины
    FactorArray<int> parent;    ///< Родитель каждой вершины в дереве путей
    int sourceVertex;           ///< Исходная вершина
    bool allReachable;          ///< Все ли вершины достижимы из источника
    
    /**
     * @brief Конструктор по умолчанию
     */
    ShortestPathResult() 
        : sourceVertex(-1)
        , allReachable(true) 
    {}
    
    /**
     * @brief Получить расстояние до вершины v
     * @param v Номер вершины
     * @return Расстояние или -1 если вне диапазона
     */
    int getDistance(int v) const {
        if (v < 0 || v >= dist.size()) return -1;
        return dist.get(v);
    }
    
    /**
     * @brief Восстановить путь от источника до вершины v
     * @param v Целевая вершина
     * @return Массив вершин пути (от source до v)
     */
    FactorArray<int> getPath(int v) const {
        FactorArray<int> path;
        
        if (v < 0 || v >= parent.size()) return path;
        
        // Собираем путь от v к source
        FactorArray<int> reversePath;
        int current = v;
        
        while (current != -1) {
            reversePath.add(current, reversePath.size());
            current = parent.get(current);
        }
        
        // Разворачиваем путь
        for (int i = reversePath.size() - 1; i >= 0; --i) {
            path.add(reversePath.get(i), path.size());
        }
        
        return path;
    }
};