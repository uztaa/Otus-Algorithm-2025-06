#include "BaseSorter.h"
#include "SortEvent.h"
#include "InsertionSorter.h"

void InsertionSorter::sort(std::vector<Record> &arr)
{
    notify(StartEvent(getName()));

    for (size_t i = 1; i < arr.size(); ++i)
    {
        size_t j = i;
        while (j > 0)
        {
            notify(CompareEvent(getName(), j, j - 1));

            if (arr[j].getKey() < arr[j - 1].getKey())
            {
                std::swap(arr[j], arr[j - 1]);
                notify(SwapEvent(getName(), j, j - 1));
                --j;
            }
            else
            {
                break;
            }
        }
    }

    notify(FinishEvent(getName()));
};
