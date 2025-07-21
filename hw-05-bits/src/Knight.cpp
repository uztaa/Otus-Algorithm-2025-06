// Knight.cpp

#include "Knight.h"
#include "BitUtils.h"

// Константы для вертикалей (границ доски)
constexpr uint64_t FILE_A = 0x0101010101010101ULL;
constexpr uint64_t FILE_B = 0x0202020202020202ULL;
constexpr uint64_t FILE_G = 0x4040404040404040ULL;
constexpr uint64_t FILE_H = 0x8080808080808080ULL;

std::pair<int, uint64_t> Knight::getMoves(int position) const {
    if (position < 0 || position > 63)
        return {0, 0};

    uint64_t knight = 1ULL << position;
    uint64_t mask = 0;

    // Ходы коня: 8 направлений, учитываем границы
    if (position < 48 && (knight & FILE_H) == 0) mask |= knight << 17;
    if (position < 48 && (knight & FILE_A) == 0) mask |= knight << 15;
    if (position > 15 && (knight & FILE_H) == 0) mask |= knight >> 15;
    if (position > 15 && (knight & FILE_A) == 0) mask |= knight >> 17;
    if (position < 56 && (knight & FILE_G) == 0 && (knight & FILE_H) == 0) mask |= knight << 10;
    if (position < 56 && (knight & FILE_A) == 0 && (knight & FILE_B) == 0) mask |= knight << 6;
    if (position > 7 && (knight & FILE_G) == 0 && (knight & FILE_H) == 0) mask |= knight >> 6;
    if (position > 7 && (knight & FILE_A) == 0 && (knight & FILE_B) == 0) mask |= knight >> 10;

    // Используем BitUtils для подсчёта количества ходов
    int count = BitUtils::popcount(mask);

    return {count, mask};
}