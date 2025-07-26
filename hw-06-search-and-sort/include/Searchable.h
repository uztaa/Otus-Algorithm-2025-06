#pragma once
#include <vector>
#include "Record.h"

/**
 * Searchable interface for searching records in a collection.
 * This interface defines a method for searching records by key.
 */
class Searchable {
public:
    /**
     * Searches for a record with the specified key in the given array of records.
     * @param arr The array of records to search in.
     * @param key The key to search for.
     * @return A pointer to the found record, or nullptr if not found.
	 */
    virtual Record* search(std::vector<Record>& arr, int key) = 0;
    virtual ~Searchable() = default;
};
