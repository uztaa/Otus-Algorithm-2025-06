#pragma once

#include "IStringSearch.hpp"
#include <string>
#include <vector>
#include <array>

/**
 * @brief Алгоритм Бойера-Мура-Хорспула (упрощённый Boyer-Moore).
 * 
 * Использует только правило плохого символа (bad character rule).
 * Сравнение символов происходит справа налево.
 * 
 * Сложность:
 * - Препроцессинг: O(m + σ), где σ — размер алфавита (256)
 * - Поиск: O(n/m) в лучшем случае, O(n*m) в худшем
 * - Память: O(σ) = O(256) = O(1)
 */
class BMHSearch : public IStringSearch {
public:
    /// Размер алфавита (ASCII)
    static constexpr size_t ALPHABET_SIZE = 256;
    
    /**
     * @brief Конструктор по умолчанию.
     */
    BMHSearch();
    
    /**
     * @brief Конструктор с установкой образца.
     * @param pattern Искомый образец
     */
    explicit BMHSearch(const std::string& pattern);
    
    void setPattern(const std::string& pattern) override;
    int search(const std::string& text) override;
    std::vector<int> searchAll(const std::string& text) override;
    
    size_t getComparisonCount() const override;
    std::string getName() const override;
    const std::string& getPattern() const override;
    void resetStats() override;

private:
    /**
     * @brief Построить таблицу плохого символа.
     * 
     * Для каждого символа вычисляет сдвиг:
     * - Если символ есть в образце (кроме последней позиции): 
     *   shift = m - 1 - последняя позиция
     * - Иначе: shift = m
     */
    void buildBadCharTable();

private:
    std::string pattern_;
    std::array<int, ALPHABET_SIZE> badCharShift_;
    mutable size_t comparisons_ = 0;
};
