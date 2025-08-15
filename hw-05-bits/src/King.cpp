// King.cpp

#include "King.h"
#include <BitUtils.h>

constexpr uint64_t FILE_A = 0x0101010101010101ULL;
constexpr uint64_t FILE_H = 0x8080808080808080ULL;

std::pair<int, uint64_t> King::getMoves(int position) const {
    if (position < 0 || position > 63)
        return {0, 0};

    uint64_t king = 1ULL << position;
    uint64_t mask = 0;

    if (position < 56) mask |= king << 8;
    if (position > 7)  mask |= king >> 8;
    if ((king & FILE_A) == 0) mask |= king >> 1;
    if ((king & FILE_H) == 0) mask |= king << 1;
    if (position < 56 && (king & FILE_A) == 0) mask |= king << 7;
    if (position < 56 && (king & FILE_H) == 0) mask |= king << 9;
    if (position > 7 && (king & FILE_A) == 0) mask |= king >> 9;
    if (position > 7 && (king & FILE_H) == 0) mask |= king >> 7;

    // Используем BitUtils для подсчёта количества ходов
    int count = BitUtils::popcount(mask);

    return {count, mask};
}