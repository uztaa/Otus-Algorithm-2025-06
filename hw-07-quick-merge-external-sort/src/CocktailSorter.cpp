#include "CocktailSorter.h"

void CocktailSorter::sort(std::vector<Record>& arr) {

        notify(StartEvent(getName()));

        if (arr.empty()) {
            notify(FinishEvent(getName()));
            return;
        }

        bool swapped = true;
        size_t start = 0;
        size_t end = arr.size() - 1;

        while (swapped) {
            swapped = false;

            for (size_t i = start; i < end; ++i) {
                notify(CompareEvent(getName(), i, i + 1));

                if (arr[i].getKey() > arr[i + 1].getKey()) {
                    std::swap(arr[i], arr[i + 1]);
                    swapped = true;
                    notify(SwapEvent(getName(), i, i + 1));
                }
            }

            if (!swapped) break;

            swapped = false;
            --end;

            for (size_t i = end; i > start; --i) {
                notify(CompareEvent(getName(), i - 1, i));

                if (arr[i - 1].getKey() > arr[i].getKey()) {
                    std::swap(arr[i - 1], arr[i]);
                    swapped = true;
                    notify(SwapEvent(getName(), i - 1, i));
                }
            }

            ++start;
        }

        notify(FinishEvent(getName()));
    };