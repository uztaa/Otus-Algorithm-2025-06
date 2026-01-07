#include "search/AutomatonSearch.hpp"
#include <algorithm>

AutomatonSearch::AutomatonSearch(const std::string& pattern) {
    setPattern(pattern);
}

void AutomatonSearch::setPattern(const std::string& pattern) {
    pattern_ = pattern;
    buildDelta();
    resetStats();
}

void AutomatonSearch::buildDelta() {
    const int m = static_cast<int>(pattern_.length());
    
    // Создаём таблицу переходов: (m+1) состояний × 256 символов
    delta_.resize(m + 1);
    for (auto& row : delta_) {
        row.fill(0);
    }
    
    // Для каждого состояния и каждого символа
    for (int state = 0; state <= m; ++state) {
        for (int c = 0; c < static_cast<int>(ALPHABET_SIZE); ++c) {
            // Строка = prefix(pattern, state) + символ c
            std::string line = pattern_.substr(0, state) + static_cast<char>(c);
            
            // Ищем максимальный k такой, что prefix(pattern, k) — суффикс line
            int k = std::min(state + 1, m);
            
            while (k > 0) {
                if (isSuffix(line, pattern_.substr(0, k))) {
                    break;
                }
                --k;
            }
            
            delta_[state][c] = k;
        }
    }
}

bool AutomatonSearch::isSuffix(const std::string& line, const std::string& prefix) const {
    if (prefix.length() > line.length()) {
        return false;
    }
    
    size_t offset = line.length() - prefix.length();
    for (size_t i = 0; i < prefix.length(); ++i) {
        if (line[offset + i] != prefix[i]) {
            return false;
        }
    }
    
    return true;
}

int AutomatonSearch::search(const std::string& text) {
    const int n = static_cast<int>(text.length());
    const int m = static_cast<int>(pattern_.length());
    
    if (m == 0) {
        return -1;
    }
    
    if (m > n) {
        return -1;
    }
    
    int state = 0;
    
    for (int i = 0; i < n; ++i) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        ++comparisons_;
        state = delta_[state][c];
        
        if (state == m) {
            return i - m + 1;
        }
    }
    
    return -1;
}

std::vector<int> AutomatonSearch::searchAll(const std::string& text) {
    std::vector<int> results;
    
    const int n = static_cast<int>(text.length());
    const int m = static_cast<int>(pattern_.length());
    
    if (m == 0 || m > n) {
        return results;
    }
    
    int state = 0;
    
    for (int i = 0; i < n; ++i) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        ++comparisons_;
        state = delta_[state][c];
        
        if (state == m) {
            results.push_back(i - m + 1);
            // Продолжаем поиск — автомат сам откатится
        }
    }
    
    return results;
}

size_t AutomatonSearch::getComparisonCount() const {
    return comparisons_;
}

std::string AutomatonSearch::getName() const {
    return "Automaton";
}

const std::string& AutomatonSearch::getPattern() const {
    return pattern_;
}

void AutomatonSearch::resetStats() {
    comparisons_ = 0;
}
