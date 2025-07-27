#include <vector>
#include <utility>
#include <string>
#include "BaseSorter.h"
#include "SortEvent.h"

/**
 * @brief Реализация шейкер-сортировки (Cocktail Sort).
 * Это улучшенный вариант пузырьковой сортировки, выполняющий проход в обе стороны.
 */
class CocktailSorter : public BaseSorter {
public:
    /**
     * @brief Сортирует вектор объектов Record по ключу.
     *
     * Алгоритм проходит по массиву сначала слева направо, затем справа налево,
     * "всплывая" минимальные и максимальные элементы к границам.
     *
     * @param arr Вектор объектов Record для сортировки.
     * @return std::pair<size_t, size_t> — количество сравнений и перестановок.
     */
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0;
        size_t swaps = 0;

        notify(StartEvent(getName()));

        if (arr.empty()) {
            notify(FinishEvent(getName()));
            return { comparisons, swaps };
        }

        bool swapped = true;
        size_t start = 0;
        size_t end = arr.size() - 1;

        while (swapped) {
            swapped = false;

            // Прямой проход слева направо
            for (size_t i = start; i < end; ++i) {
                ++comparisons;
                notify(CompareEvent(getName(), i, i + 1));

                if (arr[i].getKey() > arr[i + 1].getKey()) {
                    std::swap(arr[i], arr[i + 1]);
                    ++swaps;
                    swapped = true;
                    notify(SwapEvent(getName(), i, i + 1));
                }
            }

            if (!swapped) break;

            swapped = false;
            --end;

            // Обратный проход справа налево
            for (size_t i = end; i > start; --i) {
                ++comparisons;
                notify(CompareEvent(getName(), i - 1, i));

                if (arr[i - 1].getKey() > arr[i].getKey()) {
                    std::swap(arr[i - 1], arr[i]);
                    ++swaps;
                    swapped = true;
                    notify(SwapEvent(getName(), i - 1, i));
                }
            }

            ++start;
        }

        notify(FinishEvent(getName()));
        return { comparisons, swaps };
    }
};