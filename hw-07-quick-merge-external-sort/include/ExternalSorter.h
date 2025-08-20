// ExternalSorter.h
#pragma once

#include <string>
#include <memory>
#include <vector>
#include "FileService.h"
#include "Sortable.h"

/**
 * @brief Класс для реализации внешней сортировки.
 * 
 * В конструкторе принимает интерфейс FileService для работы с файлами,
 * и внутренний Sortable для сортировки блоков в памяти.
 * Параметр blockSize задает размер блока для локальной сортировки.
 * Параметр tmpDir задает директорию для временных файлов.
 */
class ExternalSorter {
public:

    /**
     * @brief Конструктор внешнего сортировщика.
     * 
     * @param fileService Сервис работы с файлами.
     * @param inMemSorter Сортировщик для обработки блоков в памяти.
     * @param blockSize Размер блока для сортировки в памяти.
     * @param tmpDir Путь к временному каталогу для промежуточных файлов.
     */
    explicit ExternalSorter(std::shared_ptr<FileService> fileService,
        std::shared_ptr<Sortable> inMemSorter,
        size_t blockSize = 100,
        const std::string& tmpDir = "tmp_extsort/");

    /**
     * @brief Внешний способ сортировки ES1 с разбиением входных данных на T файлов по диапазонам ключей,
     * затем сортировкой каждого подфайла внутренним сортировщиком и последовательной конкатенацией.
     * 
     * @param inputFile Путь к входному файлу с числами.
     * @param outputFile Путь к результатирующему отсортированному файлу.
     * @param T Количество подфайлов и интервалов разбиения по ключам.
     * @return true в случае успешной сортировки, false — иначе.
     */
    bool sortES1(const std::string& inputFile, const std::string& outputFile, int T);

private:
    std::shared_ptr<FileService> fs_;
    std::shared_ptr<Sortable> inMemSorter_;
    size_t blockSize_;
    std::string tmpDirectory_;

    /**
     * @brief Вспомогательный метод разбивки файла на T подфайлов по ключам.
     * @param inputFile Путь к исходному файлу.
     * @param T Количество подфайлов.
     * @param maxKey Максимальное значение ключа (числа).
     * @param chunkFiles Выходной список путей сгенерированных подфайлов.
     * @return true при успешном разбиении, false при ошибках.
     */
    bool splitFileToTChunks(const std::string& inputFile, int T, int maxKey, std::vector<std::string>& chunkFiles);

    /**
     * @brief Объединяет последовательность файлов в один, путем простой конкатенации.
     * @param chunkFiles Список файлов для объединения, предполагается что содержат уже отсортированные данные.
     * @param outputFile Итоговый файл после объединения.
     * @return true при успехе, false — при ошибках.
     */
    bool mergeChunks(const std::vector<std::string>& chunkFiles, const std::string& outputFile);
};
