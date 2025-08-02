#pragma once

#include <string>
#include "SortEvent.h"

/** 
 * @file SortEventFormatter.h
 * @brief Форматирование событий сортировки в строку.
 *
 * Этот файл содержит класс SortEventFormatter, который предоставляет статический метод для форматирования
 * событий сортировки в строку для удобного отображения или логирования.
 */
class SortEventFormatter {
public:
    static std::string format(const SortEvent& event);
};
