#include "QuickSorter.h"
#include "SortEvent.h"
#include <utility>

void QuickSorter::sort(std::vector<Record> &arr)
{
    notify(StartEvent(getName()));
    if (!arr.empty())
    {
        quickSort(arr, 0, arr.size() - 1);
    }
    notify(FinishEvent(getName()));
}

void QuickSorter::quickSort(std::vector<Record> &arr, std::size_t low, std::size_t high)
{
    if (low >= high)
        return;
    std::size_t p = partition(arr, low, high);
    if (p > 0)
        quickSort(arr, low, p - 1); // защита от underflow
    quickSort(arr, p + 1, high);
}

std::size_t QuickSorter::partition(std::vector<Record> &arr, std::size_t low, std::size_t high)
{
    // Используем схему Ломуто
    int pivot = arr[high].getKey();
    std::size_t i = low; // позиция для следующего элемента <= pivot

    for (std::size_t j = low; j < high; ++j)
    {
        notify(CompareEvent(getName(), j, high)); // сравнение с опорным
        if (arr[j].getKey() <= pivot)
        {
            if (i != j)
            {
                std::swap(arr[i], arr[j]);
                notify(SwapEvent(getName(), i, j));
            }
            ++i;
        }
    }

    if (i != high)
    {
        std::swap(arr[i], arr[high]);
        notify(SwapEvent(getName(), i, high));
    }
    return i;
}