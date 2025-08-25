#pragma once

#include <string>
#include <vector>

/**
 * @brief Интерфейс для работы с файлами.
 *
 * Абстракция для создания, чтения, записи и удаления текстовых файлов с числами.
 * Позволяет инкапсулировать обращение к файловой системе и упрощает мокирование при тестах.
 */
class FileService
{
public:
    virtual ~FileService() = default;

    /**
     * @brief Создать (очистить) файл по указанному пути.
     * @param path Путь к файлу.
     * @return true при успешном создании, false — при ошибке.
     */
    virtual bool createFile(const std::string &path) = 0;

    /**
     * @brief Записать целиком набор целых чисел в файл (перезапись/очистка).
     * @param path Путь к файлу.
     * @param lines Массив чисел для записи.
     * @return true при успешной записи, false — при ошибке.
     */
    virtual bool writeLines(const std::string &path, const std::vector<int> &lines) = 0;

    /**
     * @brief Добавить набор чисел в конец файла.
     * @param path Путь к файлу.
     * @param lines Массив чисел для добавления.
     * @return true при успешной записи, false — при ошибке.
     */
    virtual bool appendLines(const std::string &path, const std::vector<int> &lines) = 0;

    /**
     * @brief Прочитать все числа из текстового файла.
     * @param path Путь к файлу.
     * @param outLines Вектор для записи считанных чисел.
     * @return true при успешном чтении, false — при ошибке.
     */
    virtual bool readLines(const std::string &path, std::vector<int> &outLines) = 0;

    /**
     * @brief Удалить файл по пути.
     * @param path Путь к файлу.
     * @return true при успешном удалении, false — при ошибке.
     */
    virtual bool deleteFile(const std::string &path) = 0;

    /**
     * @brief Проверить, существует ли файл по пути.
     * @param path Путь к файлу.
     * @return true если файл существует, false — иначе.
     */
    virtual bool fileExists(const std::string &path) = 0;

    /**
     * @brief Вспомогательный метод разбивки файла на T подфайлов по ключам.
     * @param inputFile Путь к исходному файлу.
     * @param chunks Количество подфайлов.
     * @param maxKey Максимальное значение ключа (числа).
     * @param chunkFiles Выходной список путей сгенерированных подфайлов.
     * @return true при успешном разбиении, false при ошибках.
     */
    virtual bool splitFileToChunks(const std::string &inputFile, int chunks, int maxKey, std::vector<std::string> &chunkFiles) = 0;

    /**
     * @brief Объединяет последовательность файлов в один, путем простой конкатенации.
     * @param chunkFiles Список файлов для объединения
     * @return true при успехе, false — при ошибках.
     */
    virtual bool mergeChunks(const std::vector<std::string> &chunkFiles, const std::string &outputFile) = 0;
};
