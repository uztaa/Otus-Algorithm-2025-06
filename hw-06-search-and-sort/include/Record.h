#pragma once
#include <string>

/** 
 * @file Record.h
 * @brief Определение класса Record для хранения ключа и значения.
 * 
 * Этот класс используется для хранения пар "ключ-значение", где ключ - это целое число,
 * а значение - строка. Он может быть использован в различных структурах данных, таких как
 * ассоциативные массивы или базы данных.
 */
class Record {
private:
	// Поля для хранения ключа и значения
    int key;
    std::string value;

public:
	// Конструктор для инициализации ключа и значения
    Record(int k, std::string v) : key(k), value(std::move(v)) {}
    
    int getKey() const { return key; }
    const std::string& getValue() const { return value; }
};
