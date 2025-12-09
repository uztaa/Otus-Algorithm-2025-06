#pragma once

#include "Array.h"
#include <stdexcept>
#include <memory>

/**
 * @brief Реализация динамического массива с увеличением ёмкости в заданный фактор
 * 
 * @tparam T Тип элементов массива  
 */
template<typename T>
class FactorArray : public Array<T> {
private:
    T* data;
    int capacity;
    int count;
    const int factor = 2; // Фактор увеличения ёмкости

    void resize() {
		// Вычисляем новую ёмкость
        int newCapacity = (capacity == 0) ? 1 : capacity * factor;
        //  1. Создаём новый массив с новой ёмкостью
        std::unique_ptr<T[]> newData(new T[newCapacity]);
		// 2. Копируем данные из старого массива в новый
        for (int i = 0; i < count; ++i) {
            newData[i] = data[i];
        }
		// 3.	Удаляем старый массив
        delete[] data;
		// 4.     Переназначаем указатель на новый массив
        data = newData.release();
		// 5.  Обновляем ёмкость
        capacity = newCapacity;
    }

public:
    FactorArray() : data(nullptr), capacity(0), count(0) {}

    ~FactorArray() {
        delete[] data;
    }

    /**
     * @brief Конструктор копирования
     * @param other Другой массив для копирования
     */
    FactorArray(const FactorArray& other)
        : data(nullptr), capacity(other.capacity), count(other.count) {
        if (capacity > 0) {
            data = new T[capacity];
            for (int i = 0; i < count; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    /**
     * @brief Оператор присваивания
     * @param other Другой массив для присваивания
     * @return Ссылка на текущий объект
     */
    FactorArray& operator=(const FactorArray& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            count = other.count;
            data = (capacity > 0) ? new T[capacity] : nullptr;
            for (int i = 0; i < count; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    void add(T item, int index) override {
        Array<T>::check_index_for_insert(index, count);
        if (count >= capacity) resize();

        for (int i = count; i > index; --i) {
            data[i] = data[i - 1];
        }

        data[index] = item;
        ++count;
    }

    T remove(int index) override {
        Array<T>::check_index(index, count);

        T removed = data[index];
        for (int i = index; i < count - 1; ++i) {
            data[i] = data[i + 1];
        }
        --count;
        return removed;
    }

    T get(int index) const override {
        Array<T>::check_index(index, count);
        return data[index];
    }

    int size() const override {
        return count;
    }
};

