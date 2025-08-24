#include "BubbleSorter.h"

/**
 * @file BubbleSorter.cpp
*/
void BubbleSorter::sort(std::vector<Record> &arr)
{
    notify(StartEvent(getName()));

    for (size_t i = 0; i < arr.size(); ++i)
    {
        for (size_t j = arr.size() - 1; j > i; --j)
        {
            notify(CompareEvent(getName(), j, j - 1));

            if (arr[j].getKey() < arr[j - 1].getKey())
            {
                std::swap(arr[j], arr[j - 1]);
                notify(SwapEvent(getName(), j, j - 1));
            }
        }
    }

    notify(FinishEvent(name));
};