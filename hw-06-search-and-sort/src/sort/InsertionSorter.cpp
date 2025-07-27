#include "BaseSorter.h"
#include "SortEvent.h"

/** 
 * @file InsertionSorter.cpp
 * @brief Реализация класса InsertionSorter для сортировки вставками.
 *
 * Этот класс реализует алгоритм сортировки вставками, который последовательно проходит по массиву
 * и вставляет каждый элемент на его правильное место в отсортированной части массива.
 */
class InsertionSorter : public BaseSorter {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;

        notify(StartEvent(getName()));

        for (size_t i = 1; i < arr.size(); ++i) {
            size_t j = i;

            // Перемещаем элемент влево, пока не найдем его место
            while (j > 0) {
                ++comparisons;
                notify(CompareEvent(getName(), j, j - 1));

                if (arr[j].getKey() < arr[j - 1].getKey()) {
                    std::swap(arr[j], arr[j - 1]);
                    ++swaps;
                    notify(SwapEvent(getName(), j, j - 1));
                    --j;
                }
                else {
                    break;
                }
            }
        }

        notify(FinishEvent(getName()));
        return { comparisons, swaps };
    }

};