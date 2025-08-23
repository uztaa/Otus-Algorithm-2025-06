#include "TimeFormatter.h"
#include <iomanip>

static int64_t NS_IN_US = 1000;               // микросекунда в нс
static const int64_t NS_IN_MS = 1'000'000;    // милисекунда
static const int64_t NS_IN_S = 1'000'000'000; // секунда
static const int64_t NS_IN_M = 60 * NS_IN_S;  // минута
static const int64_t NS_IN_H = 60 * NS_IN_M;  // час

std::string TimeFormatter::formatDuration(int64_t ns) const
{
    if (ns < 0)
        return "timeout (" + formatDuration( (-1) * ns) + ")";

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);

    if (ns < NS_IN_US)
    {
        // наносекунды
        return std::to_string(ns) + " ns";
    }
    else if (ns < NS_IN_MS)
    {
        // микросекунды
        double us = ns / static_cast<double>(NS_IN_US);
        oss << us << " µs";
        return oss.str();
    }
    else if (ns < NS_IN_S)
    {
        // миллисекунды
        double ms = ns / static_cast<double>(NS_IN_MS);
        oss << ms << " ms";
        return oss.str();
    }
    else if (ns < NS_IN_M)
    {
        // секунды
        double s = ns / static_cast<double>(NS_IN_S);
        oss << s << " s";
        return oss.str();
    }
    else if (ns < NS_IN_H)
    {
        // минуты + секунды
        long long minutes = ns / NS_IN_M;
        double seconds = (ns % NS_IN_M) / static_cast<double>(NS_IN_S);
        oss << minutes << " m " << seconds << " s";
        return oss.str();
    }
    else
    {
        // часы + минуты + секунды
        long long hours = ns / NS_IN_H;
        long long minutes = (ns % NS_IN_H) / NS_IN_M;
        double seconds = (ns % NS_IN_M) / static_cast<double>(NS_IN_S);
        oss << hours << " h " << minutes << " m " << seconds << " s";
        return oss.str();
    }
}