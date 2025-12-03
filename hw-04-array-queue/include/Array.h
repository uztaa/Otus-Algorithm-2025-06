#pragma once

#include <stdexcept>

/**
 * @brief Абстрактный класс массива
 */
template<typename T>
class Array {
public:
    virtual ~Array() {}

    /**
     * @brief Добавить элемент по индексу
     * @param item Элемент для добавления
     * @param index Индекс, по которому нужно добавить элемент
     */
    virtual void add(T item, int index) = 0;

    /**
     * @brief Удалить элемент по индексу
     * @param index Индекс элемента для удаления
     * @return Удалённый элемент
     */
    virtual T remove(int index) = 0;

    /**
     * @brief Получение элемента по индексу
     * @param index Индекс элемента для удаления
     * @return Найденный элемент
     */
    virtual T get(int index) const = 0;

    /** 
     * @brief Получение текущего размера массива
     * @return Размер массива
     */
    virtual int size() const = 0;

protected:
	/**
     * @brief Проверка корректности индекса для доступа к элементу
     * @param index Индекс для проверки
     * @param count Текущий размер массива
     */
    void check_index(int index, int count) const {
        if (index < 0 || index >= count)
            throw std::out_of_range("Index out of bounds");
    }

	/**
     * @brief Проверка корректности индекса для вставки элемента
     * @param index Индекс для проверки
     * @param count Текущий размер массива
     */
    void check_index_for_insert(int index, int count) const {
        if (index < 0 || index > count)
            throw std::out_of_range("Index out of bounds");
    }
};
