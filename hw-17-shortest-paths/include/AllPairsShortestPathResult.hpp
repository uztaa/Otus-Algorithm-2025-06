#pragma once

#include "FactorArray.h"

/**
 * @brief Результат работы алгоритма Флойда-Уоршелла
 * 
 * Содержит матрицу кратчайших расстояний между всеми парами вершин
 * и матрицу предшественников для восстановления путей.
 */
struct AllPairsShortestPathResult {
    FactorArray<int> dist;      ///< Матрица расстояний (n×n, одномерный массив)
    FactorArray<int> next;      ///< Матрица следующих вершин для восстановления пути
    int vertexCount;            ///< Количество вершин
    bool hasNegativeCycle;      ///< Обнаружен ли отрицательный цикл
    
    /// Значение, обозначающее отсутствие пути
    static constexpr int INF = 1'000'000'000;
    
    /**
     * @brief Конструктор по умолчанию
     */
    AllPairsShortestPathResult() 
        : vertexCount(0)
        , hasNegativeCycle(false) 
    {}
    
    /**
     * @brief Получить расстояние между вершинами u и v
     * @return Расстояние или INF если пути нет
     */
    int getDistance(int u, int v) const {
        if (u < 0 || u >= vertexCount || v < 0 || v >= vertexCount) {
            return INF;
        }
        return dist.get(u * vertexCount + v);
    }
    
    /**
     * @brief Получить следующую вершину на пути из u в v
     * @return Следующая вершина или -1 если пути нет
     */
    int getNext(int u, int v) const {
        if (u < 0 || u >= vertexCount || v < 0 || v >= vertexCount) {
            return -1;
        }
        return next.get(u * vertexCount + v);
    }
    
    /**
     * @brief Восстановить путь между вершинами u и v
     * @return Массив вершин пути (от u до v)
     */
    FactorArray<int> getPath(int u, int v) const {
        FactorArray<int> path;
        
        if (u < 0 || u >= vertexCount || v < 0 || v >= vertexCount) {
            return path;
        }
        
        // Путь в саму себя — тривиальный путь из одной вершины
        if (u == v) {
            path.add(u, path.size());
            return path;
        }
        
        if (getNext(u, v) == -1) {
            return path;  // Пути нет
        }
        
        path.add(u, path.size());
        while (u != v) {
            u = getNext(u, v);
            if (u == -1) break;  // Защита от бесконечного цикла
            path.add(u, path.size());
        }
        
        return path;
    }
};