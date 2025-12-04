#pragma once

#include "FactorArray.h"

/**
 * @brief Структура данных Union-Find (Disjoint Set Union)
 * 
 * Реализация системы непересекающихся множеств с оптимизациями:
 * - Сжатие путей (path compression) в операции find()
 * - Объединение по рангу (union by rank) в операции merge()
 * 
 * Используется для эффективного отслеживания компонент связности
 * в алгоритмах построения минимального остовного дерева.
 * 
 * Сложность операций:
 * - find(): O(α(n)) ≈ O(1) амортизированная
 * - merge(): O(α(n)) ≈ O(1) амортизированная
 * где α(n) — обратная функция Аккермана, практически константа
 * 
 * @note Использует FactorArray из arrayslib для хранения данных
 */
class UnionFind {
public:
    /**
     * @brief Конструктор
     * @param n Количество элементов (вершин)
     * 
     * Изначально каждый элемент находится в своём собственном множестве:
     * parent[i] = i для всех i
     */
    explicit UnionFind(int n) : count_(n) {
        for (int i = 0; i < n; ++i) {
            parent_.add(i, i);  // parent[i] = i
            rank_.add(0, i);    // rank[i] = 0
        }
    }
    
    /**
     * @brief Найти корень (представителя) множества для элемента
     * 
     * Использует сжатие путей: после нахождения корня все элементы
     * на пути от v до корня напрямую привязываются к корню.
     * 
     * @param v Элемент для поиска
     * @return Корень множества, содержащего v
     * 
     * Пример:
     *   До: 0 ← 1 ← 2 ← 3  (find(3) возвращает 0)
     *   После: 0 ← 1, 0 ← 2, 0 ← 3  (все указывают на 0)
     */
    int find(int v) {
        if (parent_.get(v) != v) {
            // Рекурсивно находим корень и сжимаем путь
            int root = find(parent_.get(v));
            // Обновляем parent[v] = root (сжатие пути)
            parent_.remove(v);
            parent_.add(root, v);
        }
        return parent_.get(v);
    }
    
    /**
     * @brief Объединить два множества
     * 
     * Использует объединение по рангу: меньшее дерево присоединяется
     * к большему, что гарантирует логарифмическую высоту.
     * 
     * @param u Элемент первого множества
     * @param v Элемент второго множества
     * @return true если объединение произошло (были в разных множествах),
     *         false если уже были в одном множестве
     */
    bool merge(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        
        // Уже в одном множестве
        if (rootU == rootV) {
            return false;
        }
        
        // Объединение по рангу
        int rankU = rank_.get(rootU);
        int rankV = rank_.get(rootV);
        
        if (rankU < rankV) {
            // Присоединяем U к V
            parent_.remove(rootU);
            parent_.add(rootV, rootU);
        } else if (rankU > rankV) {
            // Присоединяем V к U
            parent_.remove(rootV);
            parent_.add(rootU, rootV);
        } else {
            // Ранги равны — присоединяем V к U и увеличиваем ранг U
            parent_.remove(rootV);
            parent_.add(rootU, rootV);
            rank_.remove(rootU);
            rank_.add(rankU + 1, rootU);
        }
        
        count_--;  // Уменьшаем число компонент
        return true;
    }
    
    /**
     * @brief Проверить, находятся ли элементы в одном множестве
     * @param u Первый элемент
     * @param v Второй элемент
     * @return true если в одном множестве
     */
    bool connected(int u, int v) {
        return find(u) == find(v);
    }
    
    /**
     * @brief Получить количество непересекающихся множеств
     * @return Число компонент связности
     */
    int componentCount() const {
        return count_;
    }
    
    /**
     * @brief Получить общее число элементов
     */
    int size() const {
        return parent_.size();
    }

private:
    FactorArray<int> parent_;  ///< parent[i] = родитель элемента i
    FactorArray<int> rank_;    ///< rank[i] = ранг (верхняя оценка высоты) дерева с корнем i
    int count_;                ///< Текущее количество компонент
};