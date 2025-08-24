#pragma once

#include "BaseSorter.h"
#include "SortEvent.h"

class InsertionShiftSorter : public BaseSorter {
public:
    void sort(std::vector<Record>& arr) override;
};