#pragma once

#include <string>

/**
 * @brief Сложение двух дробей и сокращение результата
 * 
 * Решает задачу: найти сумму дробей a/b + c/d в виде несократимой дроби x/y
 * 
 * Алгоритм:
 * 1. Парсим входную строку формата "a/b+c/d"
 * 2. Приводим к общему знаменателю: (a*d + c*b) / (b*d)
 * 3. Сокращаем на НОД(числитель, знаменатель)
 * 
 * Временная сложность: O(log(min(a,b)))
 * Пространственная сложность: O(1)
 * 
 * @note Все числа в диапазоне [1, 10000]
 * @note Результат - несократимая дробь
 * 
 * @example
 * execute("1/2+1/3") → "5/6"
 * execute("1/4+1/4") → "1/2"
 */
class TaskOneFractions {
public:
    /**
     * @brief Основной метод решения
     * 
     * @param input Входная строка формата "a/b+c/d"
     * @return std::string Результат в формате "x/y"
     */
    static std::string execute(const std::string& input);

private:
    /**
     * @brief Вычисление НОД (алгоритм Евклида)
     * 
     * @param a Первое число
     * @param b Второе число
     * @return int Наибольший общий делитель
     */
    static int gcd(int a, int b);
    
    /**
     * @brief Парсирование входной строки
     * 
     * @param input Строка "a/b+c/d"
     * @param a Первый числитель (output)
     * @param b Первый знаменатель (output)
     * @param c Второй числитель (output)
     * @param d Второй знаменатель (output)
     */
    static void parseInput(const std::string& input, int& a, int& b, int& c, int& d);
};
