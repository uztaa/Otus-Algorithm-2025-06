#include "Bishop.h"
#include "BitUtils.h"

// Слон движется по диагоналям до края доски
std::pair<int, uint64_t> Bishop::getMoves(int position) const {
    if (position < 0 || position > 63)
        return { 0, 0 };

    uint64_t mask = 0;
    int row = position / 8; // от 0 (первая горизонталь) до 7 (восьмая)
    int col = position % 8; // от 0 (файл a) до 7 (файл h)

    // Диагональ вверх-вправо
    for (int r = row + 1, f = col + 1; r <= 7 && f <= 7; ++r, ++f)
        mask |= (1ULL << (r * 8 + f));

    // Диагональ вверх-влево
    for (int r = row + 1, f = col - 1; r <= 7 && f >= 0; ++r, --f)
        mask |= (1ULL << (r * 8 + f));

    // Диагональ вниз-вправо
    for (int r = row - 1, f = col + 1; r >= 0 && f <= 7; --r, ++f)
        mask |= (1ULL << (r * 8 + f));

    // Диагональ вниз-влево
    for (int r = row - 1, f = col - 1; r >= 0 && f >= 0; --r, --f)
        mask |= (1ULL << (r * 8 + f));

    int count = BitUtils::popcount(mask);
    return { count, mask };
}
