#pragma once

#include "BaseSorter.h"
#include "SortEvent.h"

class InsertionSorter : public BaseSorter {
public:
    void sort(std::vector<Record>& arr) override;
};