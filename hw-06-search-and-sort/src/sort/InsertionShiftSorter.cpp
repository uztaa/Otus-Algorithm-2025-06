#include "BaseSorter.h"

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
        for (size_t i = 1; i < arr.size(); ++i) {
            Record temp = arr[i];
            size_t j = i;
			// Перемещение элементов влево, пока не найдём правильное место для текущего элемента
            while (j > 0 && ++comparisons && arr[j - 1].getKey() > temp.getKey()) {
                arr[j] = arr[j - 1];
                ++shifts;
                --j;
            }
            arr[j] = temp;
        }
        return { comparisons, shifts };
    }
};