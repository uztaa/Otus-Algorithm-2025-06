#pragma once

#include "SpaceArray.h"
#include "FactorArray.h"
#include <stack>

/**
 * @brief Результат работы алгоритма Тарьяна
 * 
 * Содержит разбиение графа на компоненты сильной связности (SCC).
 * Компонента сильной связности — максимальное множество вершин,
 * в котором из любой вершины можно достичь любую другую.
 */
struct TarjanResult {
    SpaceArray<int> components;    ///< components[i] = массив вершин i-й компоненты
    FactorArray<int> componentId;  ///< componentId[v] = номер компоненты для вершины v
    int componentCount;            ///< Количество компонент
};

/**
 * @brief Алгоритм Тарьяна для поиска компонент сильной связности
 * 
 * Алгоритм основан на обходе в глубину (DFS) и использует два ключевых понятия:
 * 
 * 1. disc[v] (discovery time) — время обнаружения вершины при DFS.
 *    Это просто порядковый номер, когда мы впервые посетили вершину.
 * 
 * 2. low[v] (low-link value) — минимальное время обнаружения среди всех вершин,
 *    достижимых из v через поддерево DFS и не более чем одно обратное ребро.
 *    Интуитивно: "как высоко можно подняться" из вершины v.
 * 
 * Ключевая идея: вершина v является корнем SCC тогда и только тогда,
 * когда low[v] == disc[v]. Это означает, что из v нельзя "подняться выше" —
 * все вершины, достижимые из v, образуют замкнутую компоненту.
 * 
 * Сложность: O(V + E) — каждая вершина и ребро обрабатываются ровно один раз.
 */
class TarjanSCC {
public:
    /**
     * @brief Выполнить алгоритм Тарьяна
     * 
     * @param adjList Граф в виде списка смежности (SpaceArray)
     *                adjList[v] содержит все вершины, в которые ведут рёбра из v
     * @param vertexCount Количество вершин в графе
     * @return TarjanResult Разбиение на компоненты сильной связности
     * 
     * @note Вершины нумеруются с 0
     * @note Граф должен быть ОРИЕНТИРОВАННЫМ
     */
    static TarjanResult execute(const SpaceArray<int>& adjList, int vertexCount) {
        TarjanResult result;
        result.componentCount = 0;
        
        // Обработка пустого графа
        if (vertexCount <= 0) {
            return result;
        }
        
        // Инициализация массивов
        // disc[v] = -1 означает, что вершина ещё не посещена
        FactorArray<int> disc;
        FactorArray<int> low;
        FactorArray<bool> onStack;  // Находится ли вершина в стеке
        
        for (int i = 0; i < vertexCount; ++i) {
            disc.add(-1, disc.size());
            low.add(0, low.size());
            onStack.add(false, onStack.size());
            result.componentId.add(-1, result.componentId.size());
        }
        
        std::stack<int> stack;  // Стек для сбора вершин текущего пути
        int timer = 0;          // Глобальный таймер для disc[]
        
        // Запускаем DFS из каждой непосещённой вершины
        // (граф может быть несвязным)
        for (int v = 0; v < vertexCount; ++v) {
            if (disc.get(v) == -1) {
                dfs(v, adjList, disc, low, onStack, stack, timer, result);
            }
        }
        
        return result;
    }

private:
    /**
     * @brief Рекурсивный DFS для алгоритма Тарьяна
     * 
     * При входе в вершину:
     * - Присваиваем disc[v] и low[v] текущее время
     * - Помещаем v в стек
     * 
     * При обходе соседей:
     * - Если сосед w не посещён: рекурсивно обходим, затем low[v] = min(low[v], low[w])
     * - Если сосед w в стеке: low[v] = min(low[v], disc[w]) — нашли обратное ребро
     * 
     * При выходе из вершины:
     * - Если low[v] == disc[v]: v — корень SCC, извлекаем всё из стека до v
     */
    static void dfs(int v, 
                    const SpaceArray<int>& adjList,
                    FactorArray<int>& disc,
                    FactorArray<int>& low,
                    FactorArray<bool>& onStack,
                    std::stack<int>& stack,
                    int& timer,
                    TarjanResult& result) {
        
        // Время обнаружения вершины v
        disc.remove(v);
        disc.add(timer, v);
        low.remove(v);
        low.add(timer, v);
        timer++;
        
        // Помещаем v в стек
        stack.push(v);
        onStack.remove(v);
        onStack.add(true, v);
        
        // Обходим всех соседей (если у вершины есть рёбра)
        if (v < adjList.blockCount()) {
            int neighborCount = adjList.blockSize(v);
            for (int i = 0; i < neighborCount; ++i) {
                int w = adjList.get(v, i);
                
                if (disc.get(w) == -1) {
                    // w ещё не посещена — рекурсивный вызов
                    dfs(w, adjList, disc, low, onStack, stack, timer, result);
                    
                    // После возврата обновляем low[v]
                    // low[v] = min(low[v], low[w])
                    int newLow = std::min(low.get(v), low.get(w));
                    low.remove(v);
                    low.add(newLow, v);
                }
                else if (onStack.get(w)) {
                    // w в стеке — это обратное ребро к предку в текущем пути
                    // low[v] = min(low[v], disc[w])
                    // Используем disc[w], а не low[w], чтобы не "перепрыгивать" через SCC
                    int newLow = std::min(low.get(v), disc.get(w));
                    low.remove(v);
                    low.add(newLow, v);
                }
                // Если w посещена, но не в стеке — это вершина из уже найденной SCC,
                // её игнорируем
            }
        }
        
        // Проверяем, является ли v корнем SCC
        if (low.get(v) == disc.get(v)) {
            // v — корень компоненты сильной связности
            // Извлекаем из стека все вершины до v включительно
            
            int componentIndex = result.componentCount;
            result.componentCount++;
            
            int w;
            do {
                w = stack.top();
                stack.pop();
                
                // Убираем из стека
                onStack.remove(w);
                onStack.add(false, w);
                
                // Присваиваем номер компоненты
                result.componentId.remove(w);
                result.componentId.add(componentIndex, w);
                
                // Добавляем в компоненту (блок создаётся автоматически)
                result.components.add(componentIndex, w);
                
            } while (w != v);
        }
    }
};