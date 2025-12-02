#pragma once

/**
 * @file ITrie.hpp
 * @brief Интерфейс префиксного дерева (Trie)
 * @author Anton Tobolkin
 * @date December 2025
 * 
 * Базовый интерфейс для работы с префиксным деревом.
 * Уровень: Junior
 */

#include <string>
#include <vector>


/**
 * @brief Интерфейс префиксного дерева
 * 
 * Определяет базовые операции над Trie:
 * - insert: вставка слова
 * - search: точный поиск слова
 * - startsWith: поиск по префиксу
 * - remove: удаление слова
 */
class ITrie {
public:
    virtual ~ITrie() = default;
    
    /**
     * @brief Вставить слово в дерево
     * @param word Слово для вставки (lowercase English letters)
     */
    virtual void insert(const std::string& word) = 0;
    
    /**
     * @brief Проверить наличие слова в дереве
     * @param word Искомое слово
     * @return true если слово было вставлено ранее
     */
    virtual bool search(const std::string& word) const = 0;
    
    /**
     * @brief Проверить наличие слов с данным префиксом
     * @param prefix Префикс для поиска
     * @return true если существует хотя бы одно слово с таким префиксом
     */
    virtual bool startsWith(const std::string& prefix) const = 0;
    
    /**
     * @brief Удалить слово из дерева
     * @param word Слово для удаления
     * @return true если слово было удалено
     */
    virtual bool remove(const std::string& word) = 0;
    
    /**
     * @brief Получить все слова с заданным префиксом
     * @param prefix Префикс для поиска
     * @return Вектор слов, начинающихся с prefix
     */
    virtual std::vector<std::string> getWordsWithPrefix(const std::string& prefix) const = 0;
    
    /**
     * @brief Получить количество слов в дереве
     * @return Количество уникальных слов
     */
    virtual size_t size() const = 0;
    
    /**
     * @brief Проверить, пусто ли дерево
     * @return true если дерево не содержит слов
     */
    virtual bool empty() const = 0;
    
    /**
     * @brief Очистить дерево
     */
    virtual void clear() = 0;
};