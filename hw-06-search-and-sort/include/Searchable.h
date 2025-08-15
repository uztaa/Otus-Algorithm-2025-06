#pragma once
#include <vector>
#include "Record.h"

/**
 * Интерфейс для классов, которые могут выполнять поиск записей.
 */
class Searchable {
public:
    /**
	* @brief Выполняет поиск записи по ключу в массиве записей.
	* @param arr Массив записей, в котором выполняется поиск.
	* @param key Ключ, по которому выполняется поиск.
	 */
    virtual Record* search(std::vector<Record>& arr, int key) = 0;
    virtual ~Searchable() = default;
};
