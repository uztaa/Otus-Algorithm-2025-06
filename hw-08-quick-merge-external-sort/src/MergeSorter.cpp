#include "MergeSorter.h"
#include "SortEvent.h"
#include <vector>

void MergeSorter::sort(std::vector<Record> &arr)
{
    notify(StartEvent(getName()));
    if (!arr.empty())
    {
        mergeSort(arr, 0, arr.size() - 1);
    }
    notify(FinishEvent(getName()));
}

void MergeSorter::mergeSort(std::vector<Record> &arr, std::size_t left, std::size_t right)
{
    if (left >= right)
        return;
    std::size_t mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void MergeSorter::merge(std::vector<Record> &arr, std::size_t left, std::size_t mid, std::size_t right)
{
    // Копируем половины во временные буферы
    std::vector<Record> L(arr.begin() + static_cast<std::ptrdiff_t>(left), arr.begin() + static_cast<std::ptrdiff_t>(mid + 1));
    std::vector<Record> R(arr.begin() + static_cast<std::ptrdiff_t>(mid + 1), arr.begin() + static_cast<std::ptrdiff_t>(right + 1));

    std::size_t i = 0, j = 0; // индексы в L и R
    std::vector<Record> merged;
    merged.reserve(right - left + 1);

    while (i < L.size() && j < R.size())
    {
        // Сравниваем кандидатов; указываем индексы относительно исходного массива
        notify(CompareEvent(getName(), left + i, (mid + 1) + j));
        if (L[i].getKey() <= R[j].getKey())
        {
            merged.push_back(L[i]);
            ++i;
        }
        else
        {
            merged.push_back(R[j]);
            ++j;
        }
    }
    while (i < L.size())
    {
        merged.push_back(L[i++]);
    }
    while (j < R.size())
    {
        merged.push_back(R[j++]);
    }

    // Переносим результат в исходный массив и эмитим SetEvent
    for (std::size_t k = 0; k < merged.size(); ++k)
    {
        arr[left + k] = merged[k];
        notify(SetEvent(getName(), left + k, arr[left + k]));
    }
}