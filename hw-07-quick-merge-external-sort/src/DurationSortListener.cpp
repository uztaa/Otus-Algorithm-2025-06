#include "DurationSortListener.h"

void DurationSortListener::onEvent(const SortEvent& event) {
    if (event.getType() == "start") {
        start_time_ = std::chrono::steady_clock::now();
        duration_ = std::chrono::duration<double, std::milli>::zero();
    }
    else if (event.getType() == "finish") {
        auto end_time = std::chrono::steady_clock::now();
        duration_ = end_time - start_time_;
    }
}
