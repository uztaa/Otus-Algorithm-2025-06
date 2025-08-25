#include "BinaryInsertionSorter.h"
#include "SortEvent.h"

/**
 * @file BinaryInsertionSorter.cpp
 * @brief Реализация класса BinaryInsertionSorter для сортировки с использованием бинарного поиска.
 */

size_t BinaryInsertionSorter::binarySearch(const std::vector<Record> &arr,
                                           const Record &key,
                                           size_t left,
                                           size_t right)
{
    while (left <= right)
    {
        // Вычисление среднего индекса
        size_t mid = (left + right) / 2;

        notify(CompareEvent(getName(), mid, right + 1));

        if (arr[mid].getKey() < key.getKey())
        {
            // Если ключ меньше, продолжаем поиск в правой части
            left = mid + 1;
        }
        else
        {
            // Проверка выхода за границы массива
            if (mid == 0)
                break;

            // Если ключ больше или равен текущему элементу, продолжаем поиск в левой части
            right = mid - 1;
        }
    }

    return left;
}

void BinaryInsertionSorter::sort(std::vector<Record> &arr)
{
    notify(StartEvent(getName()));

    if (arr.empty())
    {
        notify(FinishEvent(getName()));
        return;
    }

    // специальный флаг необходимости сдвига
    bool need_shift = true;

    for (size_t i = 1; i < arr.size(); ++i)
    {
        Record current_record = arr[i];

        // Поиск позиции для вставки текущего ключа
        size_t pos = binarySearch(arr, current_record, 0, i - 1);

        // Если найденная позиция — текущая, не делаем вставку
        if (pos == i)
            continue;

        // Если позиции разные, но значения совпадают
        if (arr[pos].getKey() == current_record.getKey())
        {
            // корреткируем вставку с сохранением порядка
            // Например, последняя 2-ка в массиве [1, 2, 2, 3, 3, 2] должна встать на 3-ю позицию.
            while (pos < arr.size())
            {
                notify(CompareEvent(getName(), pos, i));
                if (arr[pos].getKey() != current_record.getKey())
                {
                    break;
                }
                ++pos;
            }

            if (pos == i)
                continue;
        };

        if (pos < arr.size())
        {
            // Если позиция равна текущему индексу, ничего не делаем
            // иначе сдвигаем элементы вправо
            for (size_t j = i; j > pos; --j)
            {
                arr[j] = arr[j - 1];
                notify(SwapEvent(getName(), j, j - 1));
            }

            // Вставляем ключ на найденную позицию
            arr[pos] = current_record;
            notify(SetEvent(getName(), pos, current_record));
        }
    }

    notify(FinishEvent(getName()));
}
