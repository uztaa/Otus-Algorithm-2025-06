#pragma once

#include <string>
#include "SortEvent.h"

/** 
 * @file SortEventFormatter.h
 * @brief converter SortEvent to string for logging or other display logic.
 */
class SortEventFormatter {
public:
    static std::string format(const SortEvent& event);
};
