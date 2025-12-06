#include "Task8_LargeBarn.hpp"
#include <algorithm>
#include <stack>

/**
 * @brief Find the largest rectangle in a matrix with obstacles
 * Uses histogram approach: for each row, compute heights and find max rectangle
 */
int TaskEightLargeBarn::execute(int n, int m,
                                 const std::vector<std::pair<int, int>>& occupied) {
    // Build the height matrix
    auto heights = getHeights(n, m, occupied);
    
    int max_area = 0;
    
    // For each row, calculate the maximum rectangle in that histogram
    for (int i = 0; i < n; ++i) {
        int area = maxRectangleInHistogram(heights[i]);
        max_area = std::max(max_area, area);
    }
    
    return max_area;
}

/**
 * @brief Calculate heights for each cell (consecutive free cells above)
 */
std::vector<std::vector<int>> TaskEightLargeBarn::getHeights(
    int n, int m,
    const std::vector<std::pair<int, int>>& occupied) {
    
    // Create obstacle matrix
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m, 0));
    for (const auto& p : occupied) {
        int x = p.first;
        int y = p.second;
        if (x < n && y < m) {
            matrix[x][y] = 1;  // Mark as obstacle
        }
    }
    
    // Calculate heights - count consecutive 0s above (including current)
    std::vector<std::vector<int>> heights(n, std::vector<int>(m));
    
    // First row - height is 1 if free, 0 if obstacle
    for (int j = 0; j < m; ++j) {
        heights[0][j] = (matrix[0][j] == 0) ? 1 : 0;
    }
    
    // Remaining rows - accumulate height or reset
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] == 1) {
                // Obstacle - reset height
                heights[i][j] = 0;
            } else {
                // Free cell - increase height from above
                heights[i][j] = heights[i - 1][j] + 1;
            }
        }
    }
    
    return heights;
}

/**
 * @brief Find maximum rectangle area in histogram using monotonic stack
 * 
 * Algorithm:
 * 1. For each bar, find the left boundary where height >= current bar
 * 2. For each bar, find the right boundary where height >= current bar
 * 3. Area = height[j] * (right - left + 1)
 * 4. Return maximum area found
 */
int TaskEightLargeBarn::maxRectangleInHistogram(const std::vector<int>& height) {
    int n = height.size();
    if (n == 0) return 0;
    
    std::vector<int> L(n), R(n);
    std::stack<int> st;
    
    // Find left boundaries - for each bar, find the nearest smaller bar on left
    for (int j = 0; j < n; ++j) {
        while (!st.empty() && height[st.top()] >= height[j]) {
            st.pop();
        }
        // Left boundary: leftmost position where we can extend this bar
        L[j] = st.empty() ? 0 : st.top() + 1;
        st.push(j);
    }
    
    // Clear stack for right boundary calculation
    while (!st.empty()) st.pop();
    
    // Find right boundaries - for each bar, find the nearest smaller bar on right
    for (int j = n - 1; j >= 0; --j) {
        while (!st.empty() && height[st.top()] >= height[j]) {
            st.pop();
        }
        // Right boundary: rightmost position where we can extend this bar
        R[j] = st.empty() ? n - 1 : st.top() - 1;
        st.push(j);
    }
    
    // Calculate maximum area
    int max_area = 0;
    for (int j = 0; j < n; ++j) {
        // Width = right boundary - left boundary + 1
        int width = R[j] - L[j] + 1;
        int area = height[j] * width;
        max_area = std::max(max_area, area);
    }
    
    return max_area;
}
