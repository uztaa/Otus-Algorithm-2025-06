#pragma once

#include <string>
#include <optional>
#include <functional>

/**
 * @brief Интерфейс двоичного дерева поиска
 */
class IBinaryTree {
public:
    virtual ~IBinaryTree() = default;

    /**
     * @brief Вставка элемента
     * @param x Значение для вставки
     */
    virtual void insert(int x) = 0;

    /**
     * @brief Поиск элемента
     * @param x Искомое значение
     * @return true если элемент найден
     */
    virtual bool search(int x) const = 0;

    /**
     * @brief Удаление элемента
     * @param x Значение для удаления
     */
    virtual void remove(int x) = 0;

    /**
     * @brief Получить количество элементов
     */
    virtual size_t size() const = 0;

    /**
     * @brief Проверить, пустое ли дерево
     */
    virtual bool empty() const = 0;

    /**
     * @brief Очистить дерево
     */
    virtual void clear() = 0;

    /**
     * @brief Получить высоту дерева
     */
    virtual int height() const = 0;

    /**
     * @brief Получить имя структуры данных
     */
    virtual std::string getName() const = 0;
};