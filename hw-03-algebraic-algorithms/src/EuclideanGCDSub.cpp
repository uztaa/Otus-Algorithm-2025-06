#include "EuclideanGCDSub.h"
#include <stdexcept>
#include <cstdlib>
#include <StringToUint64Parser.h>

std::string EuclideanGCDSub::compute(const std::string& a_str, const std::string& b_str) const {
    StringToUint64Parser parser;
    uint64_t a = parser.parse_uint64(a_str);
    uint64_t b = parser.parse_uint64(b_str);

    if (a == 0) return std::to_string(b);
    if (b == 0) return std::to_string(a);

    while (a != b) {
        if (a > b)
            a -= b;
        else
            b -= a;
    }

    return std::to_string(a);
}