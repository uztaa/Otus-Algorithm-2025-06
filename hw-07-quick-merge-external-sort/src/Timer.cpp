#include "Timer.h"

Timer::Timer()
    : duration_ns_(0), running_(false)
{
}

void Timer::start()
{
    running_ = true;
    start_time_ = std::chrono::steady_clock::now();
    duration_ns_ = 0;
}

void Timer::stop()
{
    if (running_)
    {
        auto end_time = std::chrono::steady_clock::now();
        duration_ns_ = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time_).count();
        running_ = false;
    }
}

int64_t Timer::getDurationNs() const
{
    return duration_ns_;
}
