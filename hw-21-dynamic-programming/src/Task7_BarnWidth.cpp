#include "Task7_BarnWidth.hpp"
#include <stack>

void TaskSevenBarnWidth::execute(const std::vector<int>& height,
                                 std::vector<int>& L,
                                 std::vector<int>& R) {
    computeLeft(height, L);
    computeRight(height, R);
}

void TaskSevenBarnWidth::computeLeft(const std::vector<int>& height,
                                     std::vector<int>& L) {
    int n = height.size();
    L.assign(n, 0);
    
    std::stack<int> st;
    
    for (int j = 0; j < n; ++j) {
        while (!st.empty() && height[st.top()] < height[j]) {
            st.pop();
        }
        
        if (st.empty()) {
            L[j] = 0;
        } else {
            L[j] = st.top() + 1;
        }
        
        st.push(j);
    }
}

void TaskSevenBarnWidth::computeRight(const std::vector<int>& height,
                                      std::vector<int>& R) {
    int n = height.size();
    R.assign(n, n - 1);
    
    std::stack<int> st;
    
    for (int j = n - 1; j >= 0; --j) {
        while (!st.empty() && height[st.top()] < height[j]) {
            st.pop();
        }
        
        if (st.empty()) {
            R[j] = n - 1;
        } else {
            R[j] = st.top() - 1;
        }
        
        st.push(j);
    }
}
