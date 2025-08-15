#include "StringToUint64Parser.h"
#include <stdexcept>

uint64_t StringToUint64Parser::parse_uint64(const std::string& str) const {
    try {
        size_t pos = 0;
        uint64_t value = std::stoull(str, &pos, 10);
        if (pos != str.size()) {
            throw std::invalid_argument("Invalid numeric string: " + str);
        }
        return value;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse uint64_t from string" + str + ": " + e.what());
    }
}