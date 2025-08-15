#pragma once

#include <vector>
#include <utility>
#include "Record.h"
#include "BaseSorter.h"

/**
 * @brief »мплементаци€ сортировки вставками с использованием бинарного поиска.
 * @class BinaryInsertionSorter
 * @details
 * –еализует сортировку вставками с использованием бинарного поиска дл€ нахождени€ позиции вставки.
 */
class BinaryInsertionSorter : public BaseSorter {
public:
    /** 
     * @brief —ортирует массив записей с использованием бинарного поиска.
     *
     * @param arr ћассив записей дл€ сортировки.
     * @return ѕара, содержаща€ количество сравнений и сдвигов.
	 */
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override;

protected:
    /** 
     * @brief ¬ыполн€ет бинарный поиск дл€ нахождени€ позиции вставки.
     *
     * @param arr ћассив записей, в котором выполн€етс€ поиск.
     * @param key  люч, дл€ которого ищетс€ позици€ вставки.
     * @param left Ћева€ граница поиска.
     * @param right ѕрава€ граница поиска.
     * @param comparisons —сылка на переменную дл€ подсчета сравнений.
	 * @return »ндекс, где должен быть вставлен ключ.
	 */
    size_t binarySearch(const std::vector<Record>& arr,
        const Record& key,
        size_t left,
        size_t right,
        size_t& comparisons);

};

