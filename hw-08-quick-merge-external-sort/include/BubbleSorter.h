#pragma once

#include <vector>
#include "BaseSorter.h"
#include "SortEvent.h"

/**
 * @brief Пузырьковый сортировщик.
 */
class BubbleSorter : public BaseSorter
{
public:
    void sort(std::vector<Record> &arr) override;
};