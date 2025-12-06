#ifndef TASK4_ISLANDS_HPP
#define TASK4_ISLANDS_HPP

#include <vector>

/**
 * @class TaskFourIslands
 * @brief Finds the number of connected islands in a matrix using DFS
 * 
 * An island is a group of connected 1s (horizontally or vertically connected).
 * Uses depth-first search to identify connected components.
 */
class TaskFourIslands {
public:
    /**
     * @brief Count connected islands (1s) in the matrix
     * @param matrix 2D vector with 0s (water) and 1s (land)
     * @return Number of connected islands
     */
    static int execute(const std::vector<std::vector<int>>& matrix);

private:
    /**
     * @brief DFS helper to mark all cells of an island
     * @param i Row index
     * @param j Column index
     * @param matrix Input matrix
     * @param visited Visited cells tracker
     * @param n Number of rows
     * @param m Number of columns
     */
    static void dfs(int i, int j, const std::vector<std::vector<int>>& matrix,
                    std::vector<std::vector<bool>>& visited, int n, int m);
};

#endif // TASK4_ISLANDS_HPP
