#pragma once

#include <future>
#include <chrono>
#include <utility>
#include <iostream> // for throw aeception

/**
 * Запускает работу в отдельном потоке.
 * Блокирует текущий поток до успешного окончания работы или таймаута.
 * @throws исключение в случае таймаута
 */
class AsyncRunner
{
public:
    template <typename Func>
    std::pair<bool, int64_t> asyncRunWithTimeout(Func &&task, int64_t timeout_ns)
    {
        auto start = std::chrono::steady_clock::now();

        auto fut = std::async(std::launch::async, std::forward<Func>(task));

        if (fut.wait_for(std::chrono::nanoseconds(timeout_ns)) == std::future_status::timeout)
        {
            // выбрасываем исключение, чтобы прервать дальнейшую работу
            throw std::runtime_error("Timeout");
        }

        bool result = fut.get();

        auto end = std::chrono::steady_clock::now();
        int64_t duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return {result, duration_ns};
    }
};
