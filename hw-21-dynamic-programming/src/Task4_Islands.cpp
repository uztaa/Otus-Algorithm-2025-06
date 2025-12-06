#include "Task4_Islands.hpp"

/**
 * @brief DFS-based island counting - fixed to handle non-square matrices
 */
int TaskFourIslands::execute(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;
    
    int n = matrix.size();
    int m = matrix[0].size();
    
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));
    int island_count = 0;
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] == 1 && !visited[i][j]) {
                dfs(i, j, matrix, visited, n, m);
                island_count++;
            }
        }
    }
    
    return island_count;
}

/**
 * @brief Depth-first search to mark all cells of an island as visited
 */
void TaskFourIslands::dfs(int i, int j, const std::vector<std::vector<int>>& matrix,
                          std::vector<std::vector<bool>>& visited, int n, int m) {
    // Граничные условия
    if (i < 0 || i >= n || j < 0 || j >= m) return;
    
    // Если уже посещена или это не остров (0)
    if (visited[i][j] || matrix[i][j] == 0) return;
    
    // Помечаем как посещённую
    visited[i][j] = true;
    
    // Рекурсивно посещаем соседей (4 направления)
    dfs(i + 1, j, matrix, visited, n, m);  // вниз
    dfs(i - 1, j, matrix, visited, n, m);  // вверх
    dfs(i, j + 1, matrix, visited, n, m);  // вправо
    dfs(i, j - 1, matrix, visited, n, m);  // влево
}
