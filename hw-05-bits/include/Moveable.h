// Moveable.h

#pragma once
#include <cstdint>
#include <utility>

// Абстрактный класс для фигуры, способной совершать ходы
class Moveable {
public:
    virtual ~Moveable() = default;
    // Возвращает пару: (количество ходов, битовая маска ходов)
    virtual std::pair<int, uint64_t> getMoves(int position) const = 0;
};