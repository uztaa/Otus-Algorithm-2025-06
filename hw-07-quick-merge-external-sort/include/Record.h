#pragma once
#include <string>

/** 
 * @file Record.h
 * @brief class with comparable key (int)
 */
class Record {
private:
    int key;
    std::string value;

public:
    Record(int k, std::string v) : key(k), value(std::move(v)) {}
    
    int getKey() const { return key; }
    const std::string& getValue() const { return value; }
};
