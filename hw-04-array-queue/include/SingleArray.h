#pragma once

#include "Array.h"
#include <stdexcept>

// Реализация динамического массива с увеличением емкости на 1
template<typename T>
class SingleArray : public Array<T> {
private:
	// Динамический массив для хранения элементов
    T* data;
	// Текущая емкость массива и количество элементов
    int capacity;
	// Текущее количество элементов в массиве
    int count;

    void resize() {
		// создаем новый массив на capacity + 1
        T* newData = new T[capacity + 1];
		// Копируем старые данные в новый массив
        for (int i = 0; i < count; ++i) {
            newData[i] = data[i];
        }
		// Освобождаем старый массив
        delete[] data;
		// Переназначаем указатель на новый массив
        data = newData;
        // Увеличиваем емкость на 1
        ++capacity;
    }

public:
    SingleArray() : data(nullptr), capacity(0), count(0) {}

    ~SingleArray() override {
        delete[] data;
    }

    void add(T item, int index) override {
        Array<T>::check_index_for_insert(index, count);

        if (count >= capacity) {
            resize();
        }

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
