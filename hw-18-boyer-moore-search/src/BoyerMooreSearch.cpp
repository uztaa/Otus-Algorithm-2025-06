#include "search/BoyerMooreSearch.hpp"
#include <algorithm>

BoyerMooreSearch::BoyerMooreSearch() {
    badChar_.fill(-1);
}

BoyerMooreSearch::BoyerMooreSearch(const std::string& pattern) : BoyerMooreSearch() {
    setPattern(pattern);
}

void BoyerMooreSearch::setPattern(const std::string& pattern) {
    pattern_ = pattern;
    buildBadCharTable();
    buildGoodSuffixTable();
    resetStats();
}

void BoyerMooreSearch::buildBadCharTable() {
    // Сбрасываем таблицу: -1 означает "символ не встречается"
    badChar_.fill(-1);
    
    const int m = static_cast<int>(pattern_.length());
    
    // Для каждого символа сохраняем позицию последнего вхождения
    for (int i = 0; i < m; ++i) {
        unsigned char c = static_cast<unsigned char>(pattern_[i]);
        badChar_[c] = i;
    }
}

void BoyerMooreSearch::buildGoodSuffixTable() {
    const int m = static_cast<int>(pattern_.length());
    
    if (m == 0) {
        goodSuffix_.clear();
        return;
    }
    
    goodSuffix_.resize(m);
    
    // goodSuffix_[0] = 1: при 0 совпавших символов сдвигаемся на 1
    goodSuffix_[0] = 1;
    
    // Для каждой длины совпавшего суффикса вычисляем минимальный сдвиг
    for (int i = 1; i < m; ++i) {
        goodSuffix_[i] = findGoodSuffixShift(i);
    }
}

int BoyerMooreSearch::findGoodSuffixShift(int suffixLen) const {
    const int m = static_cast<int>(pattern_.length());
    
    // Ищем наименьший сдвиг k, при котором:
    // 1. Суффикс pattern[m-suffixLen..m-1] совпадает с подстрокой,
    //    начинающейся на k позиций левее
    // 2. Символ перед этой подстрокой либо другой, либо мы вышли за границу
    
    for (int k = 1; k <= m; ++k) {
        bool validShift = true;
        int matchCount = 0;
        
        // Проверяем совпадение суффикса с подстрокой на k позиций левее
        for (int j = 0; j < suffixLen; ++j) {
            int rightPos = m - 1 - j;      // Позиция в суффиксе (справа налево)
            int leftPos = rightPos - k;     // Позиция в подстроке
            
            if (leftPos < 0) {
                // Вышли за левую границу — проверяем как префикс
                break;
            }
            
            if (pattern_[leftPos] != pattern_[rightPos]) {
                // Суффикс не совпадает — этот сдвиг не подходит
                validShift = false;
                break;
            }
            
            ++matchCount;
        }
        
        if (!validShift) {
            continue;
        }
        
        // Проверка условий для валидного сдвига
        if (matchCount == suffixLen) {
            // Полное совпадение суффикса
            int preLeft = m - 1 - suffixLen - k;   // Символ перед подстрокой
            int preRight = m - 1 - suffixLen;       // Символ перед суффиксом
            
            if (preLeft < 0) {
                // Вышли за границу — это префикс, сдвиг валиден
                return k;
            }
            
            if (pattern_[preLeft] != pattern_[preRight]) {
                // Предшествующие символы разные — сдвиг валиден
                return k;
            }
            // Иначе продолжаем искать больший сдвиг
        } else {
            // Частичное совпадение — проверяем, это ли префикс
            // (вышли за границу при сравнении)
            if ((k + matchCount) >= m) {
                return k;
            }
        }
    }
    
    return m;  // Максимальный сдвиг
}

int BoyerMooreSearch::search(const std::string& text) {
    const int n = static_cast<int>(text.length());
    const int m = static_cast<int>(pattern_.length());
    
    if (m == 0) {
        return -1;
    }
    
    if (m > n) {
        return -1;
    }
    
    int t = 0;
    
    while (t <= n - m) {
        int j = m - 1;
        
        // Сравнение справа налево
        while (j >= 0) {
            ++comparisons_;
            if (text[t + j] != pattern_[j]) {
                break;
            }
            --j;
        }
        
        if (j < 0) {
            // Все символы совпали — нашли!
            return t;
        }
        
        // Вычисляем сдвиги
        int matchedLen = m - 1 - j;  // Количество совпавших символов
        
        // Сдвиг по плохому символу
        unsigned char badCharInText = static_cast<unsigned char>(text[t + j]);
        int badCharShift = j - badChar_[badCharInText];
        if (badCharShift < 1) {
            badCharShift = 1;
        }
        
        // Сдвиг по хорошему суффиксу
        int goodSuffixShift = goodSuffix_[matchedLen];
        
        // Выбираем максимальный сдвиг
        t += std::max(badCharShift, goodSuffixShift);
    }
    
    return -1;
}

std::vector<int> BoyerMooreSearch::searchAll(const std::string& text) {
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
            // После нахождения используем сдвиг по good suffix для pattern[0..m-1]
            // или минимум 1 для поиска перекрывающихся
            t += goodSuffix_[0];
        } else {
            int matchedLen = m - 1 - j;
            
            unsigned char badCharInText = static_cast<unsigned char>(text[t + j]);
            int badCharShift = j - badChar_[badCharInText];
            if (badCharShift < 1) {
                badCharShift = 1;
            }
            
            int goodSuffixShift = goodSuffix_[matchedLen];
            
            t += std::max(badCharShift, goodSuffixShift);
        }
    }
    
    return results;
}

size_t BoyerMooreSearch::getComparisonCount() const {
    return comparisons_;
}

std::string BoyerMooreSearch::getName() const {
    return "Boyer-Moore";
}

const std::string& BoyerMooreSearch::getPattern() const {
    return pattern_;
}

void BoyerMooreSearch::resetStats() {
    comparisons_ = 0;
}
