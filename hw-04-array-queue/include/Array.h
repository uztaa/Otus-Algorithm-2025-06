#pragma once

#include <stdexcept>

// Интерфейс динамического массива
template<typename T>
class Array {
public:
    virtual ~Array() {}

    // Добавление элемента item по индексу index
    virtual void add(T item, int index) = 0;

    // Удаление элемента по индексу, возвращает удаленный элемент
    virtual T remove(int index) = 0;

    // Получение элемента по индексу
    virtual T get(int index) const = 0;

    // Текущий размер массива
    virtual int size() const = 0;

protected:
	// Проверка индекса для доступа и удаления
    void check_index(int index, int count) const {
        if (index < 0 || index >= count)
            throw std::out_of_range("Index out of bounds");
    }

	// Проверка индекса для вставки
    void check_index_for_insert(int index, int count) const {
        if (index < 0 || index > count)
            throw std::out_of_range("Index out of bounds");
    }
};
