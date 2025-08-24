#include "InsertionShiftSorter.h"

/**
 * @file InsertionShiftSorter.cpp
 */
void InsertionShiftSorter::sort(std::vector<Record> &arr)
{
    notify(StartEvent(getName()));

    for (size_t i = 1; i < arr.size(); ++i)
    {
        Record temp = arr[i];
        size_t j = i;

        while (j > 0)
        {
            notify(CompareEvent(getName(), j - 1, i));

            if (arr[j - 1].getKey() > temp.getKey())
            {
                arr[j] = arr[j - 1];
                notify(SetEvent(getName(), j, arr[j]));
                --j;
            }
            else
            {
                break;
            }
        }

        arr[j] = temp;
        notify(SetEvent(getName(), j, temp));
    }

    notify(FinishEvent(getName()));
};