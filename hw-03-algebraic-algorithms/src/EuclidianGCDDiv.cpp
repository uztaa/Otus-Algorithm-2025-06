#include "EuclidianGCDDiv.h"
#include "StringToUint64Parser.h"

#include <stdexcept>
#include <cstdlib>

std::string EuclidianGCDDiv::compute(const std::string& a_str, const std::string& b_str) const {
	StringToUint64Parser parser;
	uint64_t a = parser.parse_uint64(a_str);
	uint64_t b = parser.parse_uint64(b_str);

	uint64_t result = gcd(a, b);
		
	return std::to_string(result);
}

uint64_t EuclidianGCDDiv::gcd(uint64_t a, uint64_t b) const {
	while (b != 0) {
		uint64_t temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}