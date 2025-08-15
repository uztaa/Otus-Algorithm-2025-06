#pragma once

#include <string>

class FileReader {
public:
	// Читает содержимое файла и возвращает его как строку.
	// Бросает std::runtime_error в случае ошибки.
	std::string readFile(const std::string& filename) const;
};