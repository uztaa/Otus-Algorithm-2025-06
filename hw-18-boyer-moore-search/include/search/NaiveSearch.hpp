#pragma once

#include "IStringSearch.hpp"
#include <string>
#include <vector>

/**
 * @brief Наивный алгоритм поиска подстроки (полный перебор).
 * 
 * Сложность:
 * - Препроцессинг: O(1)
 * - Поиск: O(n*m) в худшем случае, O(n) в среднем
 * - Память: O(1)
 * 
 * где n — длина текста, m — длина образца.
 */
class NaiveSearch : public IStringSearch {
public:
    /**
     * @brief Конструктор по умолчанию.
     */
    NaiveSearch() = default;
    
    /**
     * @brief Конструктор с установкой образца.
     * @param pattern Искомый образец
     */
    explicit NaiveSearch(const std::string& pattern);
    
    void setPattern(const std::string& pattern) override;
    int search(const std::string& text) override;
    std::vector<int> searchAll(const std::string& text) override;
    
    size_t getComparisonCount() const override;
    std::string getName() const override;
    const std::string& getPattern() const override;
    void resetStats() override;

private:
    std::string pattern_;
    mutable size_t comparisons_ = 0;
};
