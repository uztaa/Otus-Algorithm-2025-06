#pragma once

#include "IStringSearch.hpp"
#include <string>
#include <vector>
#include <array>

/**
 * @brief Поиск подстроки на основе конечного автомата.
 * 
 * Строит таблицу переходов δ[state][char] для образца.
 * 
 * Сложность:
 * - Препроцессинг: O(m² · |Σ|) — построение таблицы переходов
 * - Поиск: O(n) — один проход по тексту
 * - Память: O(m · |Σ|)
 */
class AutomatonSearch : public IStringSearch {
public:
    static constexpr size_t ALPHABET_SIZE = 256;
    
    AutomatonSearch() = default;
    explicit AutomatonSearch(const std::string& pattern);
    
    void setPattern(const std::string& pattern) override;
    int search(const std::string& text) override;
    std::vector<int> searchAll(const std::string& text) override;
    
    size_t getComparisonCount() const override;
    std::string getName() const override;
    const std::string& getPattern() const override;
    void resetStats() override;

private:
    /**
     * @brief Построить таблицу переходов автомата.
     */
    void buildDelta();
    
    /**
     * @brief Проверить, является ли prefix суффиксом строки line.
     */
    bool isSuffix(const std::string& line, const std::string& prefix) const;

private:
    std::string pattern_;
    std::vector<std::array<int, ALPHABET_SIZE>> delta_;
    mutable size_t comparisons_ = 0;
};
