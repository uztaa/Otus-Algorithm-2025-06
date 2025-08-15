#include "SteinGCD.h"
#include "StringToUint64Parser.h"


std::string SteinGCD::compute(const std::string& a_str, const std::string& b_str) const {
    StringToUint64Parser parser;   
    uint64_t a = parser.parse_uint64(a_str);
    uint64_t b = parser.parse_uint64(b_str);

    if (a == 0) return std::to_string(b);
    if (b == 0) return std::to_string(a);

    int shift = 0;
    while (((a | b) & 1) == 0) {
        a >>= 1;
        b >>= 1;
        ++shift;
    }

    while ((a & 1) == 0)
        a >>= 1;

    do {
        while ((b & 1) == 0)
            b >>= 1;

        if (a > b)
            std::swap(a, b);

        b -= a;
    } while (b != 0);

    return std::to_string(a << shift);
}
