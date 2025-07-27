#include "BaseSorter.h"
#include "SortEvent.h"
#include <algorithm>

/**
 * @file BubbleSorter.cpp
 * @brief Реализация класса BubbleSorter для сортировки пузырьком.
 *
 * Этот класс реализует алгоритм сортировки пузырьком, который сравнивает пары соседних элементов
 * и меняет их местами, если они расположены в неправильном порядке.
 */
class BubbleSorter : public BaseSorter {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;

        notify(StartEvent(name));

        for (size_t i = 0; i < arr.size(); ++i) {
            for (size_t j = arr.size() - 1; j > i; --j) {
                ++comparisons;
                notify(CompareEvent(name, j, j - 1));

                if (arr[j].getKey() < arr[j - 1].getKey()) {
                    std::swap(arr[j], arr[j - 1]);
                    ++swaps;
                    notify(SwapEvent(name, j, j - 1));
                }
            }
        }

        notify(FinishEvent(name));
        return { comparisons, swaps };
    }
};