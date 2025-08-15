#pragma once
#include "FactorArray.h"
#include "LinkedList.h"
#include <stdexcept>

// Реализация приоритетной очереди с использованием массива списков
template<typename T>
class PriorityQueueArrayList {
private:
	// минимальный приоритет 
    int min_priority;
    // максимальный приоритет 
    int max_priority;
    // массив списков для каждого приоритета
    FactorArray<LinkedList<T>*> buckets;

public:
    PriorityQueueArrayList(int min_priority, int max_priority)
        : min_priority(min_priority), max_priority(max_priority), buckets() {
        for (int i = 0; i <= max_priority - min_priority; ++i) {
            buckets.add(new LinkedList<T>(), i);
        }
    }

    ~PriorityQueueArrayList() {
        for (int i = 0; i < buckets.size(); ++i)
            delete buckets.get(i);
    }

    void enqueue(int priority, const T& item) {
        if (priority < min_priority || priority > max_priority)
            throw std::out_of_range("Priority out of range");

		// Вычисляем индекс в массиве buckets
		int priority_index = priority - min_priority;
		// Вычисляем индекс в списке buckets.get(priority_index)
		int bucket_index = buckets.get(priority_index)->size();

        buckets.get(priority_index)->add(item, bucket_index);
    }

    T dequeue() {
        for (int i = 0; i < buckets.size(); ++i) {
            LinkedList<T>* list = buckets.get(i);
            if (list->size() > 0) {
                return list->remove(0);
            }
        }
        throw std::out_of_range("PriorityQueue is empty");
    }

    bool empty() const {
        for (int i = 0; i < buckets.size(); ++i)
            if (buckets.get(i)->size() > 0) return false;
        return true;
    }
};