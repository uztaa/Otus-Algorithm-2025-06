#pragma once

#include <string>
#include <memory>
#include "FileService.h"

/**
 * @brief Класс генерации текстовых файлов с случайными числами.
 * 
 * Использует RandomArrayGenerator для генерации последовательности чисел с фиксированным seed.
 * Записывает целые числа в одном столбце файла.
 */
class FileGenerator {
public:
    explicit FileGenerator(std::shared_ptr<FileService> fileService);

    /**
     * @brief Генерирует файл с N строками, числа лежат в диапазоне от 1 до T.
     * 
     * @param filename Путь к создаваемому файлу.
     * @param rows Количество чисел (строк).
     * @param maxValue Максимальное значение чисел.
     * @param seed Seed для генератора случайных чисел. Используется для воспроизводимости.
     * @return true при успешном создании файла, false при ошибках.
     */
    bool generateFile(const std::string& filename, size_t rows, int maxValue, uint32_t seed);

private:
    std::shared_ptr<FileService> fs_;
};
