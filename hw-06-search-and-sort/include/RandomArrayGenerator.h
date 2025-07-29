#pragma once

#include "Record.h"
#include <vector>
#include <random>

/** 
 * @brief Класс для генерации случайных массивов.
 * 
 * Этот класс предоставляет функциональность для создания случайных массивов
 * объектов Record заданного размера. Он использует генератор случайных чисел
 * для создания уникальных ключей и значений.
 * 
 * @return std::vector<Record> Возвращает вектор случайных объектов Record.
 */
class RandomArrayGenerator {
private:
    std::mt19937 rng_;
public:
    /*
    * @brief Конструктор. Инициализирует генератор случайных чисел.
    */
    RandomArrayGenerator();

   /**
   * @brief Генерирует случайный массив заданного размера.
   * @param size Размер массива.
   * @return std::vector<Record> Возвращает вектор случайных объектов Record.
   */
    std::vector<Record> generate(std::size_t size);
};
