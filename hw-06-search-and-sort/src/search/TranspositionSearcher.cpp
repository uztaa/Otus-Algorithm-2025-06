#include "Searchable.h"

/**
 * @file TranspositionSearcher.cpp
 * @brief Implementation of the Transposition search strategy.
 * This class implements a search algorithm that moves the found record one position to the left.
 */
class TranspositionSearcher : public Searchable {
public:
    Record* search(std::vector<Record>& arr, int key) override {
        for (size_t i = 0; i < arr.size(); ++i) {
            if (arr[i].getKey() == key) {
				// If the found record is not the first one, swap it with the previous one
                if (i > 0)
                    std::swap(arr[i], arr[i - 1]);
                return &arr[i > 0 ? i - 1 : i];
            }
        }
        return nullptr;
    }
};