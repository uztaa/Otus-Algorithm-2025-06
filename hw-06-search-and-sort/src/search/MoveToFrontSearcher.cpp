#include "Searchable.h"

/** * @brief Реализация индексного поиска (Index Search)
 *
 * Алгоритм разбивает массив на блоки с фиксированным шагом и создает индексные массивы:
 * - index: массив ключей через равные интервалы
 * - pindex: позиции соответствующих ключей в исходном массиве
 * Затем линейно ищется блок, в котором может находиться искомый ключ, и в нем проводится линейный поиск.
 */
class MoveToFrontSearcher : public Searchable {
public:
    Record* search(std::vector<Record>& arr, int key) override {
        for (size_t i = 0; i < arr.size(); ++i) {
			// Проверяем, совпадает ли ключ текущей записи с искомым ключом
            if (arr[i].getKey() == key) {
                Record found = arr[i];
				// Сдвигаем все элементы влево, начиная с найденного
                for (size_t j = i; j > 0; --j)
                    arr[j] = arr[j - 1];
				// Помещаем найденный элемент в начало массива
                arr[0] = found;
                return &arr[0];
            }
        }
        return nullptr;
    }
};