#pragma once

#include <vector>
#include <utility>

/**
 * @brief Длина сарая - вычисление высот свободных клеток вверх
 * 
 * Решает задачу: для каждой клетки найти количество свободных клеток
 * в направлении "вверх"
 * 
 * DP соотношение:
 * height[i][j] = количество свободных клеток вверх от (i, j)
 * 
 * Если matrix[i][j] == 1 (занято):
 *     height[i][j] = 0
 * Иначе если i == 0 (верхний край):
 *     height[i][j] = 1
 * Иначе:
 *     height[i][j] = height[i-1][j] + 1
 * 
 * Временная сложность: O(N * M)
 * Пространственная сложность: O(N * M)
 */
class TaskSixBarnHeight {
public:
    /**
     * @brief Вычислить высоты свободных клеток вверх
     * 
     * @param n Количество строк
     * @param m Количество столбцов
     * @param occupied Список занятых клеток (x, y)
     * @return std::vector<std::vector<int>> Матрица высот
     */
    static std::vector<std::vector<int>> execute(
        int n, int m,
        const std::vector<std::pair<int, int>>& occupied);
};
