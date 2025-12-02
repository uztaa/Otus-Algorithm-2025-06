#pragma once

#include <string>

/**
 * @brief Интерфейс хеш-таблицы
 */
class IHashTable {
public:
    virtual ~IHashTable() = default;

    virtual void insert(int key, int value) = 0;
    virtual bool search(int key, int& value) const = 0;
    virtual bool remove(int key) = 0;

    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
    virtual double loadFactor() const = 0;
    virtual std::string getName() const = 0;
};
