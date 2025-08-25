#pragma once
#include "BaseSorter.h"
#include <vector>

/**
 * @file QuickSorter.h
 * @brief Быстрая сортировка по ключу Record::getKey().
 *
 * События:
 * - StartEvent/FinishEvent — на начало/конец сортировки
 * - CompareEvent(i,j) — при сравнении ключей arr[i] и arr[j]
 * - SwapEvent(i,j) — при обмене arr[i] и arr[j]
 */
class QuickSorter : public BaseSorter
{
public:
    QuickSorter() { setName("QuickSort"); }
    explicit QuickSorter(const std::string &n) { setName(n); }

    void sort(std::vector<Record> &arr) override;

private:
    void quickSort(std::vector<Record> &arr, std::size_t low, std::size_t high);
    std::size_t partition(std::vector<Record> &arr, std::size_t low, std::size_t high);
};