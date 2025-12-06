#include "Task1_Fractions.hpp"
#include <sstream>

std::string TaskOneFractions::execute(const std::string& input) {
    int a, b, c, d;
    parseInput(input, a, b, c, d);
    
    // Приведение к общему знаменателю
    int numerator = a * d + c * b;
    int denominator = b * d;
    
    // Сокращение на НОД
    int g = gcd(numerator, denominator);
    numerator /= g;
    denominator /= g;
    
    // Формирование результата
    return std::to_string(numerator) + "/" + std::to_string(denominator);
}

int TaskOneFractions::gcd(int a, int b) {
    // Алгоритм Евклида
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void TaskOneFractions::parseInput(const std::string& input, int& a, int& b, int& c, int& d) {
    // Формат: "a/b+c/d"
    size_t first_slash = input.find('/');
    size_t plus = input.find('+');
    size_t second_slash = input.rfind('/');
    
    a = std::stoi(input.substr(0, first_slash));
    b = std::stoi(input.substr(first_slash + 1, plus - first_slash - 1));
    c = std::stoi(input.substr(plus + 1, second_slash - plus - 1));
    d = std::stoi(input.substr(second_slash + 1));
}
