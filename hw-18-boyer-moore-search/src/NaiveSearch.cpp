#include "search/NaiveSearch.hpp"

NaiveSearch::NaiveSearch(const std::string& pattern) {
    setPattern(pattern);
}

void NaiveSearch::setPattern(const std::string& pattern) {
    pattern_ = pattern;
    resetStats();
}

int NaiveSearch::search(const std::string& text) {
    const int n = static_cast<int>(text.length());
    const int m = static_cast<int>(pattern_.length());
    
    // Пустой образец найден в начале
    if (m == 0) {
        return 0;
    }
    
    // Образец длиннее текста
    if (m > n) {
        return -1;
    }
    
    // Перебираем все возможные позиции
    for (int t = 0; t <= n - m; ++t) {
        int j = 0;
        
        // Сравниваем символы слева направо
        while (j < m) {
            ++comparisons_;
            if (text[t + j] != pattern_[j]) {
                break;
            }
            ++j;
        }
        
        // Все символы совпали
        if (j == m) {
            return t;
        }
    }
    
    return -1;
}

std::vector<int> NaiveSearch::searchAll(const std::string& text) {
    std::vector<int> results;
    
    const int n = static_cast<int>(text.length());
    const int m = static_cast<int>(pattern_.length());
    
    // Пустой образец — возвращаем пустой результат
    if (m == 0 || m > n) {
        return results;
    }
    
    // Перебираем все возможные позиции
    for (int t = 0; t <= n - m; ++t) {
        int j = 0;
        
        // Сравниваем символы слева направо
        while (j < m) {
            ++comparisons_;
            if (text[t + j] != pattern_[j]) {
                break;
            }
            ++j;
        }
        
        // Все символы совпали
        if (j == m) {
            results.push_back(t);
        }
    }
    
    return results;
}

size_t NaiveSearch::getComparisonCount() const {
    return comparisons_;
}

std::string NaiveSearch::getName() const {
    return "Naive";
}

const std::string& NaiveSearch::getPattern() const {
    return pattern_;
}

void NaiveSearch::resetStats() {
    comparisons_ = 0;
}
