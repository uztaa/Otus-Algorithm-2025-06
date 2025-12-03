#pragma once

#include "SpaceArray.h"
#include "FactorArray.h"
#include <stdexcept>

/**
 * @brief Результат работы алгоритма Демукрона
 */
struct DemukronResult {
    SpaceArray<int> levels;  ///< Уровни вершин: levels[k] содержит вершины уровня k
    bool hasCycle;           ///< true, если в графе обнаружен цикл
    
    DemukronResult() : levels(), hasCycle(false) {}
};

/**
 * @brief Алгоритм Демукрона для топологической сортировки с разбиением на уровни
 * 
 * Алгоритм выполняет топологическую сортировку ориентированного ациклического графа (DAG)
 * и разбивает вершины на уровни, где:
 * - Уровень 0: вершины с полустепенью захода = 0 (входы графа)
 * - Уровень k: вершины, все предшественники которых лежат на уровнях 0..k-1
 * 
 * Уровень вершины можно интерпретировать как максимальную длину пути от входов графа
 * до этой вершины.
 */
class DemukronAlgorithm {
public:
    /**
     * @brief Выполнить алгоритм Демукрона
     * 
     * @param adjList Граф в виде списка смежности (SpaceArray).
     *                adjList[v] содержит номера вершин, в которые ведут рёбра из v.
     * @param vertexCount Количество вершин в графе
     * @return DemukronResult Результат: уровни вершин или признак наличия цикла
     * 
     * @note Вершины нумеруются от 0 до vertexCount-1
     * @note Если граф содержит цикл, результат будет неполным, а hasCycle = true
     */
    static DemukronResult execute(const SpaceArray<int>& adjList, int vertexCount) {
        DemukronResult result;
        
        if (vertexCount <= 0) {
            return result;
        }
        
        // Шаг 1: Вычисляем полустепени захода (in-degree) для каждой вершины
        // M[v] = количество рёбер, входящих в вершину v
        FactorArray<int> inDegree;
        for (int i = 0; i < vertexCount; ++i) {
            inDegree.add(0, i);
        }
        
        // Для каждой вершины v проходим по её соседям и увеличиваем их in-degree
        for (int v = 0; v < vertexCount; ++v) {
            if (v < adjList.blockCount()) {
                int neighborCount = adjList.blockSize(v);
                for (int j = 0; j < neighborCount; ++j) {
                    int neighbor = adjList.get(v, j);
                    if (neighbor >= 0 && neighbor < vertexCount) {
                        // Увеличиваем in-degree соседа
                        int currentDegree = inDegree.get(neighbor);
                        // Для обновления значения нужно удалить и добавить заново
                        // (FactorArray не имеет метода set)
                        setInDegree(inDegree, neighbor, currentDegree + 1);
                    }
                }
            }
        }
        
        // Шаг 2: Массив для отслеживания обработанных вершин
        FactorArray<bool> processed;
        for (int i = 0; i < vertexCount; ++i) {
            processed.add(false, i);
        }
        
        int processedCount = 0;
        int currentLevel = 0;
        
        // Шаг 3: Основной цикл - обрабатываем уровни пока есть необработанные вершины
        while (processedCount < vertexCount) {
            // Находим все вершины с in-degree = 0, которые ещё не обработаны
            FactorArray<int> zeroInDegree;
            for (int v = 0; v < vertexCount; ++v) {
                if (!getProcessed(processed, v) && getInDegree(inDegree, v) == 0) {
                    zeroInDegree.add(v, zeroInDegree.size());
                }
            }
            
            // Если нет вершин с нулевой полустепенью захода, но есть необработанные
            // вершины - в графе есть цикл
            if (zeroInDegree.size() == 0) {
                result.hasCycle = true;
                return result;
            }
            
            // Добавляем найденные вершины на текущий уровень
            for (int i = 0; i < zeroInDegree.size(); ++i) {
                int v = zeroInDegree.get(i);
                result.levels.add(currentLevel, v);
                setProcessed(processed, v, true);
                processedCount++;
                
                // "Удаляем" вершину: уменьшаем in-degree всех её соседей
                if (v < adjList.blockCount()) {
                    int neighborCount = adjList.blockSize(v);
                    for (int j = 0; j < neighborCount; ++j) {
                        int neighbor = adjList.get(v, j);
                        if (neighbor >= 0 && neighbor < vertexCount) {
                            int currentDegree = getInDegree(inDegree, neighbor);
                            setInDegree(inDegree, neighbor, currentDegree - 1);
                        }
                    }
                }
            }
            
            currentLevel++;
        }
        
        return result;
    }

private:
    /**
     * @brief Вспомогательный метод для получения in-degree
     * (FactorArray не бросает исключение при корректном индексе)
     */
    static int getInDegree(const FactorArray<int>& arr, int index) {
        return arr.get(index);
    }
    
    /**
     * @brief Вспомогательный метод для установки in-degree
     * Поскольку FactorArray не имеет метода set, используем обходной путь
     */
    static void setInDegree(FactorArray<int>& arr, int index, int value) {
        // Удаляем старое значение и вставляем новое
        arr.remove(index);
        arr.add(value, index);
    }
    
    /**
     * @brief Вспомогательный метод для получения статуса обработки
     */
    static bool getProcessed(const FactorArray<bool>& arr, int index) {
        return arr.get(index);
    }
    
    /**
     * @brief Вспомогательный метод для установки статуса обработки
     */
    static void setProcessed(FactorArray<bool>& arr, int index, bool value) {
        arr.remove(index);
        arr.add(value, index);
    }
};