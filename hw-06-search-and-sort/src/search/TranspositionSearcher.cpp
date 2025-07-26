#include "Searchable.h"

/** * @brief Реализация поиска с транспозицией (Transposition Search)
 *
 * Алгоритм выполняет линейный поиск, но при нахождении искомого ключа перемещает его на одну позицию влево,
 * что ускоряет последующие поиски этого ключа.
 */
class TranspositionSearcher : public Searchable {
public:
    Record* search(std::vector<Record>& arr, int key) override {
        for (size_t i = 0; i < arr.size(); ++i) {
            if (arr[i].getKey() == key) {
				// Если ключ найден, перемещаем его на одну позицию влево
                if (i > 0)
                    std::swap(arr[i], arr[i - 1]);
                return &arr[i > 0 ? i - 1 : i];
            }
        }
        return nullptr;
    }
};