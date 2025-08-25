#pragma once
#include "BaseSorter.h"
#include <vector>

/**
 * @file MergeSorter.h
 * @brief Сортировка слиянием по ключу Record::getKey().
 *
 * События:
 * - StartEvent/FinishEvent — на начало/конец сортировки
 * - CompareEvent(i,j) — при сравнении кандидатов из левой/правой половин
 * - SetEvent(k, value) — при записи результирующего элемента в arr[k]
 */
class MergeSorter : public BaseSorter
{
public:
    MergeSorter() { setName("MergeSort"); }
    explicit MergeSorter(const std::string &n) { setName(n); }

    void sort(std::vector<Record> &arr) override;

private:
    void mergeSort(std::vector<Record> &arr, std::size_t left, std::size_t right);
    void merge(std::vector<Record> &arr, std::size_t left, std::size_t mid, std::size_t right);
};