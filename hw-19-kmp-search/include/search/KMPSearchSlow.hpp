#pragma once

#include "IStringSearch.hpp"
#include <string>
#include <vector>

/**
 * @brief Алгоритм Кнута-Морриса-Пратта с медленным вычислением π-функции.
 * 
 * Медленная версия вычисляет π-функцию наивным способом,
 * перебирая все возможные длины префиксов.
 * 
 * Сложность:
 * - Препроцессинг: O(m³) — медленное вычисление π-функции
 * - Поиск: O(n) — линейный проход по тексту
 * - Память: O(m)
 */
class KMPSearchSlow : public IStringSearch {
public:
    KMPSearchSlow() = default;
    explicit KMPSearchSlow(const std::string& pattern);
    
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
     * @brief Медленное вычисление π-функции.
     * 
     * Для каждой позиции i перебирает все возможные длины k
     * и проверяет совпадение prefix[0..k-1] с suffix[i-k+1..i].
     * 
     * Сложность: O(m³)
     */
    void computePrefixSlow();

private:
    std::string pattern_;
    std::vector<int> pi_;
    mutable size_t comparisons_ = 0;
};
