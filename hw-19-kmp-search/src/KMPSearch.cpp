#include "search/KMPSearch.hpp"

KMPSearch::KMPSearch(const std::string& pattern) {
    setPattern(pattern);
}

void KMPSearch::setPattern(const std::string& pattern) {
    pattern_ = pattern;
    computePrefixFast();
    resetStats();
}

void KMPSearch::computePrefixFast() {
    const int m = static_cast<int>(pattern_.length());
    pi_.assign(m, 0);
    
    if (m == 0) {
        return;
    }
    
    // π[0] = 0 по определению
    
    for (int i = 1; i < m; ++i) {
        // len — длина текущего кандидата на префикс-суффикс
        int len = pi_[i - 1];
        
        // Откатываемся по π-функции пока не найдём совпадение
        while (len > 0 && pattern_[len] != pattern_[i]) {
            len = pi_[len - 1];
        }
        
        // Если символы совпали, увеличиваем длину
        if (pattern_[len] == pattern_[i]) {
            ++len;
        }
        
        pi_[i] = len;
    }
}

int KMPSearch::search(const std::string& text) {
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

std::vector<int> KMPSearch::searchAll(const std::string& text) {
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

size_t KMPSearch::getComparisonCount() const {
    return comparisons_;
}

std::string KMPSearch::getName() const {
    return "KMP";
}

const std::string& KMPSearch::getPattern() const {
    return pattern_;
}

void KMPSearch::resetStats() {
    comparisons_ = 0;
}

const std::vector<int>& KMPSearch::getPrefixFunction() const {
    return pi_;
}
