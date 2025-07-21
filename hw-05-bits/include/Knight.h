// Knight.h

#pragma once
#include "Moveable.h"

// Класс для вычисления возможных ходов коня на шахматной доске
class Knight : public Moveable {
public:
    // Реализация виртуального метода
    std::pair<int, uint64_t> getMoves(int position) const override;
};