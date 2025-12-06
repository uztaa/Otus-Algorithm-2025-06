#include "Task2_Garland.hpp"
#include <algorithm>
#include <limits>

int TaskTwoGarland::execute(const std::vector<std::vector<int>>& tree) {
    if (tree.empty()) return 0;
    
    auto dp = computeDP(tree);
    
    // Ответ находится в последней строке
    return *std::max_element(dp.back().begin(), dp.back().end());
}

std::vector<std::vector<int>> TaskTwoGarland::computeDP(const std::vector<std::vector<int>>& tree) {
    int n = tree.size();
    std::vector<std::vector<int>> dp(n);
    
    for (int i = 0; i < n; ++i) {
        dp[i].resize(i + 1);
    }
    
    // Базовый случай
    dp[0][0] = tree[0][0];
    
    // Заполнение DP таблицы
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j <= i; ++j) {
            int max_prev = std::numeric_limits<int>::min();
            
            // Проверяем левую диагональ (i-1, j-1)
            if (j > 0 && j - 1 < dp[i-1].size()) {
                max_prev = std::max(max_prev, dp[i-1][j-1]);
            }
            
            // Проверяем вертикаль (i-1, j)
            if (j < dp[i-1].size()) {
                max_prev = std::max(max_prev, dp[i-1][j]);
            }
            
            dp[i][j] = tree[i][j] + max_prev;
        }
    }
    
    return dp;
}
