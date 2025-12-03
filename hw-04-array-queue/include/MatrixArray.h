#pragma once

#include "Array.h"
#include <stdexcept>

/**
 * @brief Класс MatrixArray, реализующий динамический массив с использованием матричной структуры
 * @tparam T Тип элементов массива
 */
template <typename T>
class MatrixArray : public Array<T>
{
private:
    /**
     * @brief Двумерный массив для хранения элементов
     */
    T **blocks;
    /**
     * @brief Размер одного блока
     */
    int blockSize = 10;
    /**
     * @brief Количество блоков
     */
    int blockCount;
    /**
     * @brief Текущий размер массива
     */
    int count;

    void allocateNewBlock()
    {
        T **newBlocks = new T *[blockCount + 1];
        for (int i = 0; i < blockCount; ++i)
        {
            newBlocks[i] = blocks[i];
        }

        // Создаем новый блок
        newBlocks[blockCount] = new T[blockSize];
        delete[] blocks;
        blocks = newBlocks;
        ++blockCount;
    }

public:
    MatrixArray() : blocks(nullptr), blockCount(0), count(0) {}

    ~MatrixArray()
    {
        for (int i = 0; i < blockCount; ++i)
        {
            delete[] blocks[i];
        }
        delete[] blocks;
    }

    void add(T item, int index) override
    {
        Array<T>::check_index_for_insert(index, count);

        if (count == blockCount * blockSize)
        {
            allocateNewBlock();
        }

        // Сдвигаем элементы вправо
        for (int i = count; i > index; --i)
        {
            set(i, get(i - 1));
        }

        set(index, item);
        ++count;
    }

    T remove(int index) override
    {
        Array<T>::check_index(index, count);

        T removed = get(index);

        for (int i = index; i < count - 1; ++i)
        {
            set(i, get(i + 1));
        }

        --count;
        return removed;
    }

    T get(int index) const override
    {
        Array<T>::check_index(index, count);
        ;

        int block = index / blockSize;
        int offset = index % blockSize;
        return blocks[block][offset];
    }

    void set(int index, T value)
    {
        int block = index / blockSize;
        int offset = index % blockSize;
        blocks[block][offset] = value;
    }

    int size() const override
    {
        return count;
    }
};
