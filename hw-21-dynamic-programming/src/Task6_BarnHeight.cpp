#include "Task6_BarnHeight.hpp"

std::vector<std::vector<int>> TaskSixBarnHeight::execute(
    int n, int m,
    const std::vector<std::pair<int, int>>& occupied) {
    
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m, 0));
    
    for (const auto& p : occupied) {
        int x = p.first;
        int y = p.second;
        if (x < n && y < m) {
            matrix[x][y] = 1;
        }
    }
    
    std::vector<std::vector<int>> heights(n, std::vector<int>(m));
    
    for (int j = 0; j < m; ++j) {
        heights[0][j] = (matrix[0][j] == 0) ? 1 : 0;
    }
    
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] == 1) {
                heights[i][j] = 0;
            } else {
                heights[i][j] = heights[i - 1][j] + 1;
            }
        }
    }
    
    return heights;
}
