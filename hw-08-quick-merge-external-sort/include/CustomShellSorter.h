#pragma once

#include "BaseSorter.h"
#include "SortEvent.h"

class CustomShellSorter : public BaseSorter
{
public:
    enum class GapStrategy
    {
        Classic,  // n/2, n/4, ..., 1
        Hibbard,  // 1, 3, 7, 15, ..., 2^k - 1
        Sedgewick // Sedgewick's sequence
    };
    CustomShellSorter(GapStrategy strategy = GapStrategy::Classic);
    void sort(std::vector<Record> &arr) override;

private:
    GapStrategy strategy_;

    std::vector<size_t> generateGaps(size_t n);
    std::vector<size_t> generateClassicGaps(size_t n);
    std::vector<size_t> generateHibbardGaps(size_t n);
    std::vector<size_t> generateSedgewickGaps(size_t n);
    size_t sedgewickGap(size_t k);
};