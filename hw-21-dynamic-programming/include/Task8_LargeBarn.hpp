#pragma once

#include <vector>
#include <utility>

/**
 * @brief Большой сарай - объединение всех этапов
 * 
 * Решает задачу: найти максимальную площадь прямоугольника в матрице
 * с препятствиями
 * 
 * Объединяет три предыдущих этапа:
 * 1. Вычисляем высоты свободных клеток вверх
 * 2. Для каждой строки используем её как основание гистограммы
 * 3. Применяем алгоритм "Largest Rectangle in Histogram"
 * 
 * Временная сложность: O(N * M)
 * Пространственная сложность: O(N * M)
 */
class TaskEightLargeBarn {
public:
    /**
     * @brief Найти максимальную площадь прямоугольника
     * 
     * @param n Количество строк
     * @param m Количество столбцов
     * @param occupied Список занятых клеток (x, y)
     * @return int Максимальная площадь
     */
    static int execute(int n, int m,
                      const std::vector<std::pair<int, int>>& occupied);

private:
    /**
     * @brief Вычислить матрицу высот
     */
    static std::vector<std::vector<int>> getHeights(
        int n, int m,
        const std::vector<std::pair<int, int>>& occupied);
    
    /**
     * @brief Найти максимум в гистограмме
     */
    static int maxRectangleInHistogram(const std::vector<int>& height);
};
