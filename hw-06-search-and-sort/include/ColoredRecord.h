#pragma once

#include "Record.h"
#include <SFML/Graphics/Color.hpp>

/** 
 * @file ColoredRecord.h
 * @brief Расширение класса Record для хранения цвета.
 *
 * Этот класс добавляет поле цвета к записи, позволяя хранить цветовую информацию
 * вместе с ключом и значением.
 */
class ColoredRecord : public Record {
private:
    /** 
     * @brief Поле для хранения цвета записи.
     * 
     * Это поле используется для хранения цвета, связанного с записью и ее позицей в массиве.
	 */
    sf::Color color = sf::Color::White;
public:
    ColoredRecord(int key, std::string value, sf::Color col = sf::Color::White)
        : Record{ key, value }, color(col) {}

    /** 
     * @brief Устанавливает цвет записи.
     * 
     * @param col Новый цвет для записи.
	 */
    void setColor(sf::Color col) {
		color = col;
	}

    /** 
     * @brief Получает цвет записи.
     * 
	 * @return Цвет записи.
     */
    sf::Color getColor() const {
        return color;
	}
};
