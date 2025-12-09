#pragma once

#include "GraphMatrix.hpp"
#include "AllPairsShortestPathResult.hpp"
#include "FactorArray.h"

/**
 * @brief Алгоритм Флойда-Уоршелла для поиска кратчайших путей между всеми парами вершин
 * 
 * Находит кратчайшие пути между ВСЕМИ парами вершин графа.
 * Поддерживает отрицательные веса рёбер и обнаруживает отрицательные циклы.
 * 
 * Сложность:
 * - Время: O(V³)
 * - Память: O(V²)
 * 
 * Преимущества:
 * - Простая реализация (три вложенных цикла)
 * - Работает с отрицательными весами
 * - Находит ВСЕ кратчайшие пути за один запуск
 * 
 * Недостатки:
 * - Кубическая сложность — не подходит для больших графов
 * - Использует O(V²) памяти
 */
class FloydWarshall {
public:
    /**
     * @brief Выполнить алгоритм Флойда-Уоршелла
     * 
     * @param graph Граф в виде матрицы смежности
     * @return AllPairsShortestPathResult Матрица кратчайших расстояний
     */
    static AllPairsShortestPathResult execute(const GraphMatrix& graph) {
        int n = graph.size();
        AllPairsShortestPathResult result;
        result.vertexCount = n;
        result.hasNegativeCycle = false;
        
        if (n == 0) {
            return result;
        }
        
        // Инициализация матриц dist и next
        initializeMatrices(graph, result);
        
        // Основной алгоритм: три вложенных цикла
        // k — промежуточная вершина
        // Проверяем: можно ли улучшить путь i->j, пройдя через k?
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    int distIK = getMatrixValue(result.dist, n, i, k);
                    int distKJ = getMatrixValue(result.dist, n, k, j);
                    int distIJ = getMatrixValue(result.dist, n, i, j);
                    
                    // Проверка на переполнение и улучшение пути
                    if (distIK != AllPairsShortestPathResult::INF && 
                        distKJ != AllPairsShortestPathResult::INF) {
                        int newDist = distIK + distKJ;
                        if (newDist < distIJ) {
                            setMatrixValue(result.dist, n, i, j, newDist);
                            // next[i][j] = next[i][k] — идём через k
                            int nextIK = getMatrixValue(result.next, n, i, k);
                            setMatrixValue(result.next, n, i, j, nextIK);
                        }
                    }
                }
            }
        }
        
        // Проверка на отрицательные циклы: если dist[i][i] < 0
        for (int i = 0; i < n; ++i) {
            if (getMatrixValue(result.dist, n, i, i) < 0) {
                result.hasNegativeCycle = true;
                break;
            }
        }
        
        return result;
    }

private:
    /**
     * @brief Инициализация матриц расстояний и следующих вершин
     */
    static void initializeMatrices(const GraphMatrix& graph, 
                                   AllPairsShortestPathResult& result) {
        int n = graph.size();
        int totalSize = n * n;
        
        // Инициализируем массивы
        for (int i = 0; i < totalSize; ++i) {
            result.dist.add(AllPairsShortestPathResult::INF, i);
            result.next.add(-1, i);
        }
        
        // Заполняем начальные значения из графа
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int w = graph.getWeight(i, j);
                
                if (i == j) {
                    // Расстояние до себя = 0
                    setMatrixValue(result.dist, n, i, j, 0);
                } else if (w != GraphMatrix::INF) {
                    // Есть ребро — записываем его вес
                    setMatrixValue(result.dist, n, i, j, w);
                    setMatrixValue(result.next, n, i, j, j);
                }
            }
        }
    }
    
    /**
     * @brief Получить значение из одномерного массива как из матрицы
     */
    static int getMatrixValue(const FactorArray<int>& arr, int n, int i, int j) {
        return arr.get(i * n + j);
    }
    
    /**
     * @brief Установить значение в одномерном массиве как в матрице
     */
    static void setMatrixValue(FactorArray<int>& arr, int n, int i, int j, int value) {
        int idx = i * n + j;
        arr.remove(idx);
        arr.add(value, idx);
    }
};