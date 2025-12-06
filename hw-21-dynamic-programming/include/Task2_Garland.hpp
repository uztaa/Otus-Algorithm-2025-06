#pragma once

#include <vector>

/**
 * @brief Найти максимальную сумму гирлянды в треугольной ёлочке
 * 
 * Классическая задача ДП на треугольнике (Triangle DP)
 * 
 * ДП соотношение:
 * dp[i][j] = tree[i][j] + max(dp[i-1][j-1], dp[i-1][j])
 * 
 * Временная сложность: O(N²)
 * Пространственная сложность: O(N²) или O(N) с оптимизацией
 * 
 * @note Вершины движутся от верхушки (позиция 0) к подножью
 * @note На каждом уровне i есть i+1 элементов
 * @note Движение: влево-вниз или вправо-вниз
 * 
 * @example
 * Tree: [[1], [2, 3], [4, 5, 6]]
 * Максимум: 1 + 3 + 6 = 10
 */
class TaskTwoGarland {
public:
    /**
     * @brief Найти максимальную сумму гирлянды
     * 
     * @param tree Треугольная матрица (tree[i].size() == i+1)
     * @return int Максимальная сумма
     */
    static int execute(const std::vector<std::vector<int>>& tree);

private:
    /**
     * @brief DP таблица для хранения максимальных сумм
     */
    static std::vector<std::vector<int>> computeDP(const std::vector<std::vector<int>>& tree);
};
