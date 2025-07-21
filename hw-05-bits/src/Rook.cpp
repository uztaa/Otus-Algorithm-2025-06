#include "Rook.h"
#include "BitUtils.h"

// Ладья движется по вертикали и горизонтали до края доски
std::pair<int, uint64_t> Rook::getMoves(int position) const {
    if (position < 0 || position > 63)
        return { 0, 0 };

    uint64_t mask = 0;
    int row = position / 8;
    int col = position % 8;

    // Движение вверх
    for (int r = row + 1; r <= 7; ++r)
        mask |= (1ULL << (r * 8 + col));

    // Движение вниз
    for (int r = row - 1; r >= 0; --r)
        mask |= (1ULL << (r * 8 + col));

    // Движение вправо
    for (int f = col + 1; f <= 7; ++f)
        mask |= (1ULL << (row * 8 + f));

    // Движение влево
    for (int f = col - 1; f >= 0; --f)
        mask |= (1ULL << (row * 8 + f));

    int count = BitUtils::popcount(mask);
    return { count, mask };
}
