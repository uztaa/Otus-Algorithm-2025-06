#include "search/BMHSearch.hpp"

BMHSearch::BMHSearch() {
    badCharShift_.fill(0);
}

BMHSearch::BMHSearch(const std::string& pattern) : BMHSearch() {
    setPattern(pattern);
}

void BMHSearch::setPattern(const std::string& pattern) {
    pattern_ = pattern;
    buildBadCharTable();
    resetStats();
}

void BMHSearch::buildBadCharTable() {
    const int m = static_cast<int>(pattern_.length());
    
    // По умолчанию: сдвиг на длину образца
    // (символ не встречается в образце)
    badCharShift_.fill(m > 0 ? m : 1);
    
    // Заполняем для символов в образце (кроме последнего!)
    // Если символ встречается несколько раз, берём самое правое вхождение
    for (int i = 0; i < m - 1; ++i) {
        unsigned char c = static_cast<unsigned char>(pattern_[i]);
        badCharShift_[c] = m - 1 - i;
    }
}

int BMHSearch::search(const std::string& text) {
    const int n = static_cast<int>(text.length());
    const int m = static_cast<int>(pattern_.length());
    
    if (m == 0) {
        return -1;
    }
    
    // Образец длиннее текста
    if (m > n) {
        return -1;
    }
    
    int t = 0;  // Текущая позиция в тексте
    
    while (t <= n - m) {
        int j = m - 1;  // Начинаем сравнение с конца образца
        
        // Сравниваем символы справа налево
        while (j >= 0) {
            ++comparisons_;
            if (text[t + j] != pattern_[j]) {
                break;
            }
            --j;
        }
        
        // Все символы совпали
        if (j < 0) {
            return t;
        }
        
        // Сдвиг по последнему символу окна в тексте
        unsigned char lastChar = static_cast<unsigned char>(text[t + m - 1]);
        t += badCharShift_[lastChar];
    }
    
    return -1;
}

std::vector<int> BMHSearch::searchAll(const std::string& text) {
    std::vector<int> results;
    
    const int n = static_cast<int>(text.length());
    const int m = static_cast<int>(pattern_.length());
    
    if (m == 0 || m > n) {
        return results;
    }
    
    int t = 0;
    
    while (t <= n - m) {
        int j = m - 1;
        
        while (j >= 0) {
            ++comparisons_;
            if (text[t + j] != pattern_[j]) {
                break;
            }
            --j;
        }
        
        if (j < 0) {
            results.push_back(t);
            // После нахождения сдвигаемся минимум на 1
            // (для поиска перекрывающихся вхождений)
            ++t;
        } else {
            unsigned char lastChar = static_cast<unsigned char>(text[t + m - 1]);
            t += badCharShift_[lastChar];
        }
    }
    
    return results;
}

size_t BMHSearch::getComparisonCount() const {
    return comparisons_;
}

std::string BMHSearch::getName() const {
    return "BMH";
}

const std::string& BMHSearch::getPattern() const {
    return pattern_;
}

void BMHSearch::resetStats() {
    comparisons_ = 0;
}
