#pragma once

#include <chrono>

class Timer
{
public:
    Timer();

    // Запускает или сбрасывает таймер
    void start();

    // Останавливает таймер и фиксирует время
    void stop();

    // Возвращает продолжительность работы в наносекундах
    int64_t getDurationNs() const;

private:
    std::chrono::steady_clock::time_point start_time_;
    int64_t duration_ns_;
    bool running_;
};
