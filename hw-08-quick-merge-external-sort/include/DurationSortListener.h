#pragma once

#include "SortListener.h"
#include "SortEvent.h"
#include <chrono>

/**
 * Слушатель для подсчета времени выполнения сортировки.
 * Используется в бенчмарках.
 */
class DurationSortListener : public SortListener {
public:
    DurationSortListener() : duration_(std::chrono::duration<double, std::milli>::zero()) {}

    void onEvent(const SortEvent& event) override;

    // Возвращает длительность сортировки в миллисекундах с плавающей точкой
    std::chrono::duration<double, std::milli> getDuration() const {
        return duration_;
    }

private:
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::duration<double, std::milli> duration_;
};
