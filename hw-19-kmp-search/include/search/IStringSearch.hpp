#pragma once

#include <string>
#include <vector>

/**
 * @brief Интерфейс алгоритма поиска подстроки в тексте.
 * 
 * Использование:
 * @code
 * auto searcher = std::make_unique<BoyerMooreSearch>();
 * searcher->setPattern("KOLOKOL");  // Строит таблицы сдвигов
 * int pos = searcher->search("KOLKOKOLOKOLL");
 * auto all = searcher->searchAll("KOLOKOL KOLOKOL");
 * 
 * // Переиспользование с другим pattern
 * searcher->setPattern("ABC");
 * pos = searcher->search("ABCABCABC");
 * @endcode
 */
class IStringSearch {
public:
    virtual ~IStringSearch() = default;
    
    /**
     * @brief Установить образец для поиска.
     * 
     * Строит необходимые таблицы сдвигов (препроцессинг).
     * Сбрасывает счётчик сравнений.
     * 
     * @param pattern Искомый образец
     */
    virtual void setPattern(const std::string& pattern) = 0;
    
    /**
     * @brief Поиск первого вхождения образца в тексте.
     * 
     * @param text Текст для поиска
     * @return Позиция первого вхождения (0-based) или -1 если не найден
     */
    virtual int search(const std::string& text) = 0;
    
    /**
     * @brief Поиск всех вхождений образца в тексте.
     * 
     * @param text Текст для поиска
     * @return Вектор позиций всех вхождений (0-based)
     */
    virtual std::vector<int> searchAll(const std::string& text) = 0;
    
    /**
     * @brief Получить количество сравнений символов.
     * 
     * Счётчик накапливается между вызовами search/searchAll.
     * Сбрасывается при вызове setPattern() или resetStats().
     * 
     * @return Количество сравнений
     */
    virtual size_t getComparisonCount() const = 0;
    
    /**
     * @brief Получить название алгоритма.
     * @return Название алгоритма (например, "Boyer-Moore")
     */
    virtual std::string getName() const = 0;
    
    /**
     * @brief Получить текущий образец.
     * @return Ссылка на текущий образец
     */
    virtual const std::string& getPattern() const = 0;
    
    /**
     * @brief Сбросить счётчик сравнений.
     */
    virtual void resetStats() = 0;
};
