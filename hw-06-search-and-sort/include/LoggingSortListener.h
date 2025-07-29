#pragma once

#include "SortListener.h"
#include "SortEvent.h"
#include <iostream>

/** 
 * @brief Класс для логирования событий сортировки.
 * 
 * Этот класс реализует интерфейс SortListener и выводит информацию о событиях сортировки в стандартный поток вывода.
 */
class LoggingSortListener : public SortListener {
public:
    void onEvent(const SortEvent& event) override;
};
