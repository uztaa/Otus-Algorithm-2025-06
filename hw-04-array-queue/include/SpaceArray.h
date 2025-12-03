#pragma once
#include "FactorArray.h"
#include "LinkedList.h"
#include <stdexcept>

/**
 * @brief Класс разреженного массива, состоящего из блоков-списков
 * @tparam T Тип элементов массива
 */
template <typename T>
class SpaceArray
{
private:
    FactorArray<LinkedList<T> *> blocks;

public:
    SpaceArray() : blocks() {}

    ~SpaceArray()
    {
        for (int i = 0; i < blocks.size(); ++i)
            delete blocks.get(i);
    }

    void add(int blockIndex, const T &item)
    {
        if (blockIndex < 0)
            throw std::out_of_range("Block index out of range");

        while (blocks.size() <= blockIndex)
            blocks.add(new LinkedList<T>(), blocks.size());
        blocks.get(blockIndex)->add(item, blocks.get(blockIndex)->size());
    }

    T get(int blockIndex, int index) const
    {
        if (blockIndex < 0 || blockIndex >= blocks.size())
            throw std::out_of_range("Block index out of range");
        return blocks.get(blockIndex)->get(index);
    }

    T remove(int blockIndex, int index)
    {
        if (blockIndex < 0 || blockIndex >= blocks.size())
            throw std::out_of_range("Block index out of range");
        return blocks.get(blockIndex)->remove(index);
    }

    int blockCount() const
    {
        return blocks.size();
    }

    int blockSize(int blockIndex) const
    {
        if (blockIndex < 0 || blockIndex >= blocks.size())
            throw std::out_of_range("Block index out of range");
        return blocks.get(blockIndex)->size();
    }
};