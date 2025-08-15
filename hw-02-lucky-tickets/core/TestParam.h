#pragma once

#include <iostream>

/* Параметры для теста */
class TestParam {
private:
    /* имя теста */
    std::string name;
    /* числовое представление in */
    int in;
    /* числовое представление out */
    int64_t out;

  public:
      TestParam() = default;
      TestParam(const std::string& name, int in, int64_t out);
    
      // геттеры
    std::string getName() const;
    int getIn() const;
    int64_t getOut() const;

};
