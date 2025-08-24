#include "BaseSorter.h"
#include "SortEvent.h"
#include <vector>
#include <utility>
#include <functional>
#include <cmath>

#include "CustomShellSorter.h"

CustomShellSorter::CustomShellSorter(GapStrategy strategy)
    : strategy_(strategy)
{
}

void CustomShellSorter::sort(std::vector<Record> &arr)
{
    std::vector<size_t> gaps = generateGaps(arr.size());

    notify(StartEvent(getName()));

    for (size_t gap : gaps)
    {
        for (size_t i = gap; i < arr.size(); ++i)
        {
            size_t j = i;
            while (j >= gap)
            {
                notify(CompareEvent(getName(), j - gap, j));

                if (arr[j - gap].getKey() > arr[j].getKey())
                {
                    std::swap(arr[j], arr[j - gap]);
                    notify(SwapEvent(getName(), j, j - gap));
                    j -= gap;
                }
                else
                {
                    break;
                }
            }
        }
    }

    notify(FinishEvent(getName()));
}

std::vector<size_t> CustomShellSorter::generateGaps(size_t n)
{
    switch (strategy_)
    {
    case GapStrategy::Hibbard:
        return generateHibbardGaps(n);
    case GapStrategy::Sedgewick:
        return generateSedgewickGaps(n);
    case GapStrategy::Classic:
    default:
        return generateClassicGaps(n);
    }
}

std::vector<size_t> CustomShellSorter::generateClassicGaps(size_t n)
{
    std::vector<size_t> gaps;
    for (size_t gap = n / 2; gap > 0; gap /= 2)
        gaps.push_back(gap);
    return gaps;
}

std::vector<size_t> CustomShellSorter::generateHibbardGaps(size_t n)
{
    std::vector<size_t> gaps;
    for (size_t k = 1; (1ULL << k) - 1 <= n; ++k)
        gaps.insert(gaps.begin(), (1ULL << k) - 1); // prepend
    return gaps;
}

std::vector<size_t> CustomShellSorter::generateSedgewickGaps(size_t n)
{
    std::vector<size_t> gaps;
    size_t k = 0;
    size_t gap;
    while ((gap = sedgewickGap(k)) < n)
    {
        gaps.insert(gaps.begin(), gap); // prepend
        ++k;
    }
    return gaps;
}

size_t CustomShellSorter::sedgewickGap(size_t k)
{
    if (k % 2 == 0) // even
        return 9 * ((1ULL << k) - (1ULL << (k / 2))) + 1;
    else
        return 8 * (1ULL << k) - 6 * (1ULL << ((k + 1) / 2)) + 1;
};
