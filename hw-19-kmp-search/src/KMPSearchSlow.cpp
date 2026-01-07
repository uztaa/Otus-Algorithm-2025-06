#include "search/KMPSearchSlow.hpp"

KMPSearchSlow::KMPSearchSlow(const std::string& pattern) {
    setPattern(pattern);
}

void KMPSearchSlow::setPattern(const std::string& pattern) {
    pattern_ = pattern;
    computePrefixSlow();
    resetStats();
}

void KMPSearchSlow::computePrefixSlow() {
    const int m = static_cast<int>(pattern_.length());
    pi_.assign(m, 0);
    
    if (m == 0) {
        return;
    }
    
    // π[0] = 0 по определению (нет собственного префикса)
    
    // Для каждой позиции i от 1 до m-1
    for (int i = 1; i < m; ++i) {
        // Перебираем все возможные длины k от i до 1
        // (k не может быть больше i, иначе не собственный префикс)
        for (int k = i; k > 0; --k) {
            // Проверяем: pattern[0..k-1] == pattern[i-k+1..i]
            bool match = true;
            
            for (int j = 0; j < k; ++j) {
                if (pattern_[j] != pattern_[i - k + 1 + j]) {
                    match = false;
                    break;
                }
            }
            
            if (match) {
                pi_[i] = k;
                break;  // Нашли максимальное k
            }
        }
        // Если не нашли, pi_[i] остаётся 0
    }
}

int KMPSearchSlow::search(const std::string& text) {
    const int n = static_cast<int>(text.length());
    const int m = static_cast<int>(pattern_.length());
    
    if (m == 0) {
        return -1;
    }
    
    if (m > n) {
        return -1;
    }
    
    int j = 0;  // Текущая позиция в образце
    
    for (int i = 0; i < n; ++i) {
        // Откатываемся пока не найдём совпадение или j = 0
        while (j > 0 && pattern_[j] != text[i]) {
            ++comparisons_;
            j = pi_[j - 1];
        }
        
        ++comparisons_;
        
        // Если символы совпали
        if (pattern_[j] == text[i]) {
            ++j;
        }
        
        // Если дошли до конца образца — нашли
        if (j == m) {
            return i - m + 1;
        }
    }
    
    return -1;
}

std::vector<int> KMPSearchSlow::searchAll(const std::string& text) {
    std::vector<int> results;
    
    const int n = static_cast<int>(text.length());
    const int m = static_cast<int>(pattern_.length());
    
    if (m == 0 || m > n) {
        return results;
    }
    
    int j = 0;
    
    for (int i = 0; i < n; ++i) {
        while (j > 0 && pattern_[j] != text[i]) {
            ++comparisons_;
            j = pi_[j - 1];
        }
        
        ++comparisons_;
        
        if (pattern_[j] == text[i]) {
            ++j;
        }
        
        if (j == m) {
            results.push_back(i - m + 1);
            j = pi_[j - 1];  // Продолжаем поиск
        }
    }
    
    return results;
}

size_t KMPSearchSlow::getComparisonCount() const {
    return comparisons_;
}

std::string KMPSearchSlow::getName() const {
    return "KMP-Slow";
}

const std::string& KMPSearchSlow::getPattern() const {
    return pattern_;
}

void KMPSearchSlow::resetStats() {
    comparisons_ = 0;
}

const std::vector<int>& KMPSearchSlow::getPrefixFunction() const {
    return pi_;
}
