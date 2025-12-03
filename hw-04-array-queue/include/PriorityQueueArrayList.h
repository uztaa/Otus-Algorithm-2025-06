#pragma once
#include "FactorArray.h"
#include "LinkedList.h"
#include <stdexcept>

/**
 * @brief Класс приоритетной очереди, реализованной с помощью массива списков
 * @tparam T Тип элементов в очереди
 */
template <typename T>
class PriorityQueueArrayList
{
private:
    /**
     * @brief Минимальный приоритет
     */
    int min_priority;
    /**
     * @brief Максимальный приоритет
     */
    int max_priority;
    /**
     * @brief Массив списков для хранения элементов по приоритетам
     */
    FactorArray<LinkedList<T> *> buckets;

public:
    PriorityQueueArrayList(int min_priority, int max_priority)
        : min_priority(min_priority), max_priority(max_priority), buckets()
    {
        for (int i = 0; i <= max_priority - min_priority; ++i)
        {
            buckets.add(new LinkedList<T>(), i);
        }
    }

    ~PriorityQueueArrayList()
    {
        for (int i = 0; i < buckets.size(); ++i)
            delete buckets.get(i);
    }

    void enqueue(int priority, const T &item)
    {
        if (priority < min_priority || priority > max_priority)
            throw std::out_of_range("Priority out of range");

        int priority_index = priority - min_priority;

        int bucket_index = buckets.get(priority_index)->size();

        buckets.get(priority_index)->add(item, bucket_index);
    }

    T dequeue()
    {
        for (int i = 0; i < buckets.size(); ++i)
        {
            LinkedList<T> *list = buckets.get(i);
            if (list->size() > 0)
            {
                return list->remove(0);
            }
        }
        throw std::out_of_range("PriorityQueue is empty");
    }

    bool empty() const
    {
        for (int i = 0; i < buckets.size(); ++i)
            if (buckets.get(i)->size() > 0)
                return false;
        return true;
    }
};