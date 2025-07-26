#include <vector>
#include <cstddef>
#include "record.h"
#include "searchable.h"

/**
 * @brief Реализация индексного поиска (Index Search)
 *
 * Алгоритм разбивает массив на блоки с фиксированным шагом и создает индексные массивы:
 * - index: массив ключей через равные интервалы
 * - pindex: позиции соответствующих ключей в исходном массиве
 * Затем линейно ищется блок, в котором может находиться искомый ключ, и в нем проводится линейный поиск.
 */
class IndexSearcher : public Searchable {
public:
    /**
     * Поиск записи с заданным ключом среди массива записей.
     *
     * @param arr Вектор объектов Record, в которых будет производиться поиск.
     * @param key Ключ, по которому выполняется поиск.
     * @return Указатель на найденную запись или nullptr, если не найдена.
     */
    Record* search(std::vector<Record>& arr, int key) override {
        if (arr.empty()) return nullptr;

        const size_t step = 8;
        size_t indexCount = arr.size() / step + 1;

        std::vector<int> index(indexCount);      // Значения ключей через шаг
        std::vector<size_t> pindex(indexCount);  // Соответствующие позиции

        size_t i = 0;
        for (size_t k = 0; k < arr.size(); k += step) {
            index[i] = arr[k].getKey();   // Заполняем индекс ключами
            pindex[i] = k;                // Запоминаем позиции
            ++i;
        }

        // Определяем блок, в котором потенциально находится ключ
        size_t block = 0;
        for (block = 0; block < i; ++block) {
            if (key <= index[block])
                break;
        }

        size_t beg = 0;
        size_t end = arr.size();

        // Уточняем границы блока
        if (block < i) {
            if (key == index[block]) {
                return &arr[pindex[block]];
            }
            end = pindex[block];
        }
        if (block > 0) {
            beg = pindex[block - 1];
        }

        // Линейный поиск внутри блока
        for (size_t j = beg; j < end; ++j) {
            if (arr[j].getKey() == key) {
                return &arr[j];
            }
        }

        return nullptr;  // Ключ не найден
    }
};
