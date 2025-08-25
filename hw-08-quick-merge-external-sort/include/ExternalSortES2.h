/*
Для реализации класса ExternalSortES2, который выполняет внешний способ сортировки с двумя вспомогательными файлами, можно ориентироваться на структуру ExternalSortES1, но с заменой логики использования нескольких chunk-файлов на метод с двумя файлами-помощниками.

Типичная идея ES2 внешней сортировки — это циклическое чередование двух временных файлов для слияния и разбиения, то есть:

    Размер блоков (run length), которые уже отсортированы и которые сливаются, увеличивается с каждой итерацией

    На каждой итерации чтение идет из одного файла, а результат слияния записывается в другой файл

    По завершении всех итераций отсортированный результат оказывается в одном из двух временных файлов, который копируем/переименовываем в итоговый output

Шаги реализации ExternalSortES2:

    Создаем класс ExternalSortES2, реализующий интерфейс ExternalSort.

    Используем FileService для работы с файлами.

    В методе externalSort:

        Читаем весь input файл в память по частям (runs).

        Сортируем их внутренним сортировщиком.

        Записываем отсортированные блоки последовательно в один из двух временных файлов.

        Затем циклически сливаем эти блоки с помощью двух файлов-помощников:

            Читаем по одному run с каждого временного файла, сливаем их, записываем результат в другой файл.

            Увеличиваем длину run.

        Продолжаем до тех пор пока длина run не будет покрывать весь файл.

        Последний файл (с результатом) переименовываем в outputFile.

*/

#pragma once

#include "ExternalSort.h"
#include <memory>
#include "FileService.h"
#include "Sortable.h"
#include <string>

class ExternalSortES2 : public ExternalSort {
public:
    ExternalSortES2(std::shared_ptr<FileService> fileService, std::shared_ptr<Sortable> inMemSorter)
        : fs_(std::move(fileService)), inMemSorter_(std::move(inMemSorter)) {}

    bool externalSort(const std::string& inputFile, const std::string& outputFile, int /*number_of_chunks*/, int /*maxValue*/) override;

private:
    std::shared_ptr<FileService> fs_;
    std::shared_ptr<Sortable> inMemSorter_;
};
