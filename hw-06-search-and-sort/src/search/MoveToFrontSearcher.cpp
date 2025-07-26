#include "Searchable.h"

/** 
* @file MoveToFrontSearcher.cpp
* @brief Implementation of the Move-to-Front search strategy.
*/
class MoveToFrontSearcher : public Searchable {
public:
    Record* search(std::vector<Record>& arr, int key) override {
        for (size_t i = 0; i < arr.size(); ++i) {
			// Check if the current record's key matches the search key
            if (arr[i].getKey() == key) {
                Record found = arr[i];
				// Shift all elements before the found record to the right
                for (size_t j = i; j > 0; --j)
                    arr[j] = arr[j - 1];
                // Move the found record to the front of the array
                arr[0] = found;
                return &arr[0];
            }
        }
        return nullptr;
    }
};