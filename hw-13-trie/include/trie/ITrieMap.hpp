#pragma once

/**
 * @file ITrieMap.hpp
 * @brief Интерфейс ассоциативного массива на основе Trie
 * @author Anton
 * @date December 2025
 * 
 * Ассоциативный массив (key-value store) на базе префиксного дерева.
 * Уровень: Middle
 */


#include <string>
#include <vector>
#include <optional>
#include <utility>


/**
 * @brief Интерфейс ассоциативного массива на Trie
 * @tparam V Тип значения
 * 
 * Расширение Trie для хранения пар ключ-значение.
 * Ключи — строки, значения — произвольного типа V.
 */
template<typename V>
class ITrieMap {
public:
    virtual ~ITrieMap() = default;
    
    /**
     * @brief Добавить или обновить значение по ключу
     * @param key Ключ (строка из lowercase English letters)
     * @param value Значение для сохранения
     */
    virtual void put(const std::string& key, const V& value) = 0;
    
    /**
     * @brief Получить значение по ключу
     * @param key Ключ для поиска
     * @return Значение или std::nullopt если ключ не найден
     */
    virtual std::optional<V> get(const std::string& key) const = 0;
    
    /**
     * @brief Удалить ключ и его значение
     * @param key Ключ для удаления
     * @return true если ключ был удалён
     */
    virtual bool remove(const std::string& key) = 0;
    
    /**
     * @brief Проверить наличие ключа
     * @param key Ключ для проверки
     * @return true если ключ существует
     */
    virtual bool contains(const std::string& key) const = 0;
    
    /**
     * @brief Получить все пары с ключами, начинающимися с префикса
     * @param prefix Префикс для поиска
     * @return Вектор пар (ключ, значение)
     */
    virtual std::vector<std::pair<std::string, V>> getByPrefix(const std::string& prefix) const = 0;
    
    /**
     * @brief Получить все ключи с заданным префиксом
     * @param prefix Префикс для поиска
     * @return Вектор ключей
     */
    virtual std::vector<std::string> keysWithPrefix(const std::string& prefix) const = 0;
    
    /**
     * @brief Получить количество элементов
     * @return Количество пар ключ-значение
     */
    virtual size_t size() const = 0;
    
    /**
     * @brief Проверить, пуст ли массив
     * @return true если массив пуст
     */
    virtual bool empty() const = 0;
    
    /**
     * @brief Очистить массив
     */
    virtual void clear() = 0;
    
    /**
     * @brief Получить все ключи
     * @return Вектор всех ключей
     */
    virtual std::vector<std::string> keys() const = 0;
};