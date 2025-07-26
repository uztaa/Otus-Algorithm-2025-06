#include "Searchable.h"

/** 
* @brief Класс для последовательного поиска в массиве
* Этот класс реализует интерфейс Searchable и предоставляет метод для поиска записи в неотсортированном массиве.
* @details Простой перебор массива слева направо. Применяется, когда массив не отсортирован. Самый медленный, но надёжный метод.
*/
class SequentialSearcher : public Searchable {
public:
    Record* search(std::vector<Record>& arr, int searchKey) override {
		// Поиск записи с заданным ключом в векторе
        for (auto& rec : arr) {
			// Если ключ совпадает, возвращаем указатель на запись
            if (rec.getKey() == searchKey)
                return &rec;
        }
        return nullptr;
    }
};
