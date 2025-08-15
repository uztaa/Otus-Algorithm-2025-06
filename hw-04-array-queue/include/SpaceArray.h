#pragma once
#include "FactorArray.h"
#include "LinkedList.h"
#include <stdexcept>

// Массив массивов с неполным заполнением
template<typename T>
class SpaceArray {
private:
    FactorArray<LinkedList<T>*> blocks;

public:
    SpaceArray() : blocks() {}

    ~SpaceArray() {
        for (int i = 0; i < blocks.size(); ++i)
            delete blocks.get(i);
    }

    // Добавить элемент в блок с номером blockIndex
    void add(int blockIndex, const T& item) {
        if (blockIndex < 0)
            throw std::out_of_range("Block index out of range");
        // Если блок не существует, создаём пустые блоки до нужного индекса
        while (blocks.size() <= blockIndex)
            blocks.add(new LinkedList<T>(), blocks.size());
        blocks.get(blockIndex)->add(item, blocks.get(blockIndex)->size());
    }

    // Получить элемент из блока blockIndex по индексу index
    T get(int blockIndex, int index) const {
        if (blockIndex < 0 || blockIndex >= blocks.size())
            throw std::out_of_range("Block index out of range");
        return blocks.get(blockIndex)->get(index);
    }

    // Удалить элемент из блока blockIndex по индексу index
    T remove(int blockIndex, int index) {
        if (blockIndex < 0 || blockIndex >= blocks.size())
            throw std::out_of_range("Block index out of range");
        return blocks.get(blockIndex)->remove(index);
    }

    // Количество блоков
    int blockCount() const {
        return blocks.size();
    }

    // Количество элементов в блоке
    int blockSize(int blockIndex) const {
        if (blockIndex < 0 || blockIndex >= blocks.size())
            throw std::out_of_range("Block index out of range");
        return blocks.get(blockIndex)->size();
    }
};