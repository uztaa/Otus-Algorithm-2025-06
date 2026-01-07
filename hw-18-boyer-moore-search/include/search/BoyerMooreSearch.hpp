#pragma once

#include "IStringSearch.hpp"
#include <string>
#include <vector>
#include <array>

/**
 * @brief Полный алгоритм Бойера-Мура.
 * 
 * Использует два правила сдвига:
 * 1. Bad Character Rule (правило плохого символа) — сдвиг по суффиксу текста
 * 2. Good Suffix Rule (правило хорошего суффикса) — сдвиг по префиксу шаблона
 * 
 * При каждом несовпадении выбирается максимальный из двух сдвигов.
 * 
 * Сложность:
 * - Препроцессинг: O(m + σ)
 * - Поиск: O(n/m) в лучшем случае, O(n + m) в худшем
 * - Память: O(m + σ)
 */
class BoyerMooreSearch : public IStringSearch {
public:
    /// Размер алфавита (ASCII)
    static constexpr size_t ALPHABET_SIZE = 256;
    
    /**
     * @brief Конструктор по умолчанию.
     */
    BoyerMooreSearch();
    
    /**
     * @brief Конструктор с установкой образца.
     * @param pattern Искомый образец
     */
    explicit BoyerMooreSearch(const std::string& pattern);
    
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
     * Хранит позицию последнего вхождения каждого символа в образце.
     * Если символа нет — значение -1.
     */
    void buildBadCharTable();
    
    /**
     * @brief Построить таблицу хорошего суффикса.
     * 
     * goodSuffix_[i] — минимальный сдвиг при совпадении i символов
     * с конца образца.
     */
    void buildGoodSuffixTable();
    
    /**
     * @brief Найти минимальный сдвиг для заданной длины суффикса.
     * @param suffixLen Длина совпавшего суффикса
     * @return Минимальный безопасный сдвиг
     */
    int findGoodSuffixShift(int suffixLen) const;

private:
    std::string pattern_;
    std::array<int, ALPHABET_SIZE> badChar_;  ///< Позиция последнего вхождения символа
    std::vector<int> goodSuffix_;              ///< Таблица сдвигов по суффиксу
    mutable size_t comparisons_ = 0;
};
