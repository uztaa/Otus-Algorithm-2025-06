#include "BaseSorter.h"
#include "SortEvent.h"
#include <vector>
#include <utility>
#include <functional>
#include <cmath>

/**
 * @brief ShellSorter — реализация сортировки Шелла с возможностью выбора стратегии разрывов.
 */
class CustomShellSorter : public BaseSorter {
public:
    enum class GapStrategy {
        Classic,   // n/2, n/4, ..., 1
        Hibbard,   // 1, 3, 7, 15, ..., 2^k - 1
        Sedgewick  // Sedgewick's sequence
    };

    CustomShellSorter(GapStrategy strategy = GapStrategy::Classic)
        : strategy_(strategy) {
    }

    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;
        std::vector<size_t> gaps = generateGaps(arr.size());

        notify(StartEvent(getName()));

        for (size_t gap : gaps) {
            for (size_t i = gap; i < arr.size(); ++i) {
                size_t j = i;
                while (j >= gap) {
                    ++comparisons;
                    notify(CompareEvent(getName(), j - gap, j));

                    if (arr[j - gap].getKey() > arr[j].getKey()) {
                        std::swap(arr[j], arr[j - gap]);
                        ++swaps;
                        notify(SwapEvent(getName(), j, j - gap));
                        j -= gap;
                    }
                    else {
                        break;
                    }
                }
            }
        }

        notify(FinishEvent(getName()));
        return { comparisons, swaps };
    }

private:
    GapStrategy strategy_;

	// Создание разрывов в зависимости от выбранной стратегии
    std::vector<size_t> generateGaps(size_t n) {
        switch (strategy_) {
        case GapStrategy::Hibbard: return generateHibbardGaps(n);
        case GapStrategy::Sedgewick: return generateSedgewickGaps(n);
        case GapStrategy::Classic:
        default: return generateClassicGaps(n);
        }
    }

	// Генерация разрывов для классической сортировки Шелла
    std::vector<size_t> generateClassicGaps(size_t n) {
        std::vector<size_t> gaps;
        for (size_t gap = n / 2; gap > 0; gap /= 2)
            gaps.push_back(gap);
        return gaps;
    }

	// Генерация разрывов по Хиббарду
    std::vector<size_t> generateHibbardGaps(size_t n) {
        std::vector<size_t> gaps;
        for (size_t k = 1; (1ULL << k) - 1 <= n; ++k)
            gaps.insert(gaps.begin(), (1ULL << k) - 1); // prepend
        return gaps;
    }

	// Генерация разрывов по Седжвика
    std::vector<size_t> generateSedgewickGaps(size_t n) {
        std::vector<size_t> gaps;
        size_t k = 0;
        size_t gap;
        while ((gap = sedgewickGap(k)) < n) {
            gaps.insert(gaps.begin(), gap); // prepend
            ++k;
        }
        return gaps;
    }

	// Функция для вычисления разрыва по Седжвику
    size_t sedgewickGap(size_t k) {
        if (k % 2 == 0) // even
            return 9 * ((1ULL << k) - (1ULL << (k / 2))) + 1;
        else
            return 8 * (1ULL << k) - 6 * (1ULL << ((k + 1) / 2)) + 1;
    }
};
