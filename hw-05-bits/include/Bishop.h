// Bishop.h
#pragma once
#include "Moveable.h"

// Класс для вычисления возможных ходов слона на шахматной доске
class Bishop : public Moveable {
public:
    std::pair<int, uint64_t> getMoves(int position) const override;
};