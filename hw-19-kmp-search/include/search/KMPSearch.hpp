#pragma once

#include "IStringSearch.hpp"
#include <string>
#include <vector>

/**
 * @brief Алгоритм Кнута-Морриса-Пратта с быстрым вычислением π-функции.
 * 
 * Эффективная версия вычисляет π-функцию за линейное время,
 * используя ранее вычисленные значения.
 * 
 * Сложность:
 * - Препроцессинг: O(m) — быстрое вычисление π-функции
 * - Поиск: O(n) — линейный проход по тексту
 * - Общая: O(n + m) — оптимальная асимптотика
 * - Память: O(m)
 */
class KMPSearch : public IStringSearch {
public:
    KMPSearch() = default;
    explicit KMPSearch(const std::string& pattern);
    
    void setPattern(const std::string& pattern) override;
    int search(const std::string& text) override;
    std::vector<int> searchAll(const std::string& text) override;
    
    size_t getComparisonCount() const override;
    std::string getName() const override;
    const std::string& getPattern() const override;
    void resetStats() override;
    
    /**
     * @brief Получить вычисленную π-функцию.
     * @return Вектор значений π-функции
     */
    const std::vector<int>& getPrefixFunction() const;

private:
    /**
     * @brief Быстрое вычисление π-функции.
     * 
     * Использует наблюдение: если π[i-1] = k, то при вычислении π[i]
     * мы можем использовать уже известные значения для "отката".
     * 
     * Сложность: O(m) — амортизированная линейная
     */
    void computePrefixFast();

private:
    std::string pattern_;
    std::vector<int> pi_;
    mutable size_t comparisons_ = 0;
};
