#pragma once

#include "Array.h"
#include <stdexcept>

// Реализация массива массивов (матрица)
template<typename T>
class MatrixArray : public Array<T> {
private:
    T** blocks;         // массив указателей на блоки
    int blockSize = 10; // размер одного блока
    int blockCount;     // кол-во выделенных блоков
    int count;          // общее число элементов

    void allocateNewBlock() {
        T** newBlocks = new T * [blockCount + 1];
        for (int i = 0; i < blockCount; ++i) {
            newBlocks[i] = blocks[i];
        }

        newBlocks[blockCount] = new T[blockSize]; // новый блок
        delete[] blocks;
        blocks = newBlocks;
        ++blockCount;
    }

public:
    MatrixArray() : blocks(nullptr), blockCount(0), count(0) {}

    ~MatrixArray() {
        for (int i = 0; i < blockCount; ++i) {
            delete[] blocks[i];
        }
        delete[] blocks;
    }

    void add(T item, int index) override {
        Array<T>::check_index_for_insert(index, count);

        if (count == blockCount * blockSize) {
            allocateNewBlock();
        }

        // сдвигаем элементы вправо начиная с конца
        for (int i = count; i > index; --i) {
            set(i, get(i - 1));
        }

        set(index, item);
        ++count;
    }

    T remove(int index) override {
        Array<T>::check_index(index, count);

        T removed = get(index);

        for (int i = index; i < count - 1; ++i) {
            set(i, get(i + 1));
        }

        --count;
        return removed;
    }

    T get(int index) const override {
        Array<T>::check_index(index, count);;

        int block = index / blockSize;
        int offset = index % blockSize;
        return blocks[block][offset];
    }

    void set(int index, T value) {
        int block = index / blockSize;
        int offset = index % blockSize;
        blocks[block][offset] = value;
    }

    int size() const override {
        return count;
    }
};

