#include "Task5_SmallBarn.hpp"
#include <algorithm>

int TaskFiveSmallBarn::execute(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty()) return 0;
    
    int n = matrix.size();
    int m = matrix[0].size();
    int max_area = 0;
    
    for (int i1 = 0; i1 < n; ++i1) {
        for (int j1 = 0; j1 < m; ++j1) {
            for (int i2 = i1; i2 < n; ++i2) {
                for (int j2 = j1; j2 < m; ++j2) {
                    if (isRectangleEmpty(matrix, i1, j1, i2, j2)) {
                        int area = (i2 - i1 + 1) * (j2 - j1 + 1);
                        max_area = std::max(max_area, area);
                    }
                }
            }
        }
    }
    
    return max_area;
}

bool TaskFiveSmallBarn::isRectangleEmpty(const std::vector<std::vector<int>>& matrix,
                                          int i1, int j1, int i2, int j2) {
    for (int i = i1; i <= i2; ++i) {
        for (int j = j1; j <= j2; ++j) {
            if (matrix[i][j] == 1) return false;
        }
    }
    return true;
}
