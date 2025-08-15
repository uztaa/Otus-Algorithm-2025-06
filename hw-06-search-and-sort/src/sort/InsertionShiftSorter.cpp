#include "BaseSorter.h"
#include "SortEvent.h"

/** 
 * @file InsertionShiftSorter.cpp
 * @brief Реализация класса InsertionShiftSorter для сортировки вставками с использованием сдвигов.
 *
 * Этот класс реализует алгоритм сортировки вставками, который перемещает элементы влево,
 * чтобы вставить текущий элемент на его правильное место в отсортированной части массива.
 */
class InsertionShiftSorter : public BaseSorter {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, shifts = 0;

        notify(StartEvent(getName()));

        for (size_t i = 1; i < arr.size(); ++i) {
            Record temp = arr[i];
            size_t j = i;

            while (j > 0) {
                ++comparisons;
                notify(CompareEvent(getName(), j - 1, i));

                if (arr[j - 1].getKey() > temp.getKey()) {
                    arr[j] = arr[j - 1];
                    ++shifts;
                    notify(SetEvent(getName(), j, arr[j]));
                    --j;
                }
                else {
                    break;
                }
            }

            arr[j] = temp;
            notify(SetEvent(getName(), j, temp));
        }

        notify(FinishEvent(getName()));
        return { comparisons, shifts };
    }

};