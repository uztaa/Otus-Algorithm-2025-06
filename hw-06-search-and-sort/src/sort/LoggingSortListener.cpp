#include "LoggingSortListener.h"
#include "SortEventFormatter.h"

void LoggingSortListener::onEvent(const SortEvent& event) {
    std::cout << SortEventFormatter::format(event) << std::endl;
}