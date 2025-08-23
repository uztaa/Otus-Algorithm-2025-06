#pragma once

#include <future>
#include <chrono>
#include <utility>

class AsyncRunner
{
public:
    template <typename Func>
    std::pair<bool, int64_t> asyncRunWithTimeout(Func &&task, int64_t timeout_ns)
    {
        std::cout << "Current thread ID: " << std::this_thread::get_id() << " [started]" << std::endl;

        auto start = std::chrono::steady_clock::now();

        auto fut = std::async(std::launch::async, std::forward<Func>(task));

        if (fut.wait_for(std::chrono::nanoseconds(timeout_ns)) == std::future_status::timeout)
        {
            std::cout << "Current thread ID: " << std::this_thread::get_id() << " [break]" << std::endl;
            auto end = std::chrono::steady_clock::now();
            int64_t duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            return {false, (-1) * duration_ns}; // таймаут (передаем продолжительность работы с минусом)
        }

        bool result = fut.get();

        auto end = std::chrono::steady_clock::now();
        int64_t duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        std::cout << "Current thread ID: " << std::this_thread::get_id() << " [finished]" << std::endl;

        return {result, duration_ns};
    }
};
