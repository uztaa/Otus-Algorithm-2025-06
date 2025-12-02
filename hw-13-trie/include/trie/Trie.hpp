#pragma once

/**
 * @file Trie.hpp
 * @brief Реализация префиксного дерева
 * @author Anton
 * @date December 2025
 * 
 * Реализация базового Trie для хранения и поиска строк.
 * Уровень: Junior
 */


#include "ITrie.hpp"
#include "TrieNode.hpp"
#include <memory>


/**
 * @brief Реализация префиксного дерева
 * 
 * Поддерживает операции:
 * - insert: O(m)
 * - search: O(m)
 * - startsWith: O(m)
 * - remove: O(m)
 * - getWordsWithPrefix: O(m + k), где k — кол-во результатов
 * 
 * m — длина слова/префикса
 */
class Trie : public ITrie {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    Trie();
    
    /**
     * @brief Деструктор
     */
    ~Trie() override = default;
    
    // Запрет копирования
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
    
    // Разрешение перемещения
    Trie(Trie&&) noexcept = default;
    Trie& operator=(Trie&&) noexcept = default;
    
    // ITrie interface
    void insert(const std::string& word) override;
    bool search(const std::string& word) const override;
    bool startsWith(const std::string& prefix) const override;
    bool remove(const std::string& word) override;
    std::vector<std::string> getWordsWithPrefix(const std::string& prefix) const override;
    size_t size() const override;
    bool empty() const override;
    void clear() override;
    
private:
    std::unique_ptr<TrieNode> root_;
    size_t wordCount_;
    
    /**
     * @brief Найти узел по строке
     * @param str Строка для поиска
     * @return Указатель на узел или nullptr
     */
    TrieNode* findNode(const std::string& str) const;
    
    /**
     * @brief Рекурсивный сбор слов
     * @param node Текущий узел
     * @param current Текущий префикс
     * @param results Вектор результатов
     */
    void collectWords(TrieNode* node, std::string& current, 
                      std::vector<std::string>& results) const;
    
    /**
     * @brief Рекурсивное удаление слова
     * @param node Текущий узел
     * @param word Удаляемое слово
     * @param depth Текущая глубина
     * @return true если узел можно удалить
     */
    bool removeHelper(TrieNode* node, const std::string& word, size_t depth);
};
