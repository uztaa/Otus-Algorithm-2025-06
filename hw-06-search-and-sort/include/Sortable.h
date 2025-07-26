#pragma once
#include <vector>
#include <utility>
#include "record.h"

/** 
* @brief Abstract base class for sorting
* This class defines the interface for sorting algorithms.
* It provides a pure virtual method `sort` that takes a vector of `Record`
* and returns a pair containing the number of comparisons and swaps made during sorting.
*/
class Sortable {
public:
    /**
     * @brief Sorts a vector of Record objects.
     * 
     * @param arr The vector of Record objects to be sorted.
     * @return A pair containing the number of comparisons and swaps made during sorting.
	 */
    virtual std::pair<size_t, size_t> sort(std::vector<Record>& arr) = 0;
    virtual ~Sortable() = default;
};
