#pragma once
#include <vector>
#include "Record.h"

/**
 * Interface for search data.
 */
class Searchable {
public:
    /**
	* @brief serach Record into array
	* @param arr data
	* @param key key for search
	 */
    virtual Record* search(std::vector<Record>& arr, int key) = 0;
    virtual ~Searchable() = default;
};
