#pragma once
#include <string>

class TimeFormatter
{
public:
    /**
     * @brief Функция форматирования времени.
     * Конвретирует наносекунды в читабельный формат строки.
     * Используется для бенчмаков.
     * @param ns - время в наносекундах
     * @return читабельный формат времени
     */
    std::string formatDuration(int64_t ns) const;
};