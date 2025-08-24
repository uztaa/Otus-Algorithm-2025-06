#pragma once

#include <vector>
#include <utility>
#include <string>
#include "BaseSorter.h"
#include "SortEvent.h"

/**
 * @brief Сортировщик (Cocktail Sort).
 */
class CocktailSorter : public BaseSorter {
public:
    void sort(std::vector<Record>& arr) override;
};