#include "Task3_FiveEight.hpp"

long long TaskTheFiveEight::execute(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 2;  // "5" и "8"
    if (n == 2) return 4;  // "55", "58", "85", "88"
    
    // dp[i][d1][d2] = кол-во i-значных чисел, где последние две цифры d1, d2
    // 0 - начало, 1 - цифра 5, 2 - цифра 8
    
    // Используем прокатывающийся массив для экономии памяти
    long long dp[3][3] = {0};
    long long new_dp[3][3] = {0};
    
    // Базовый случай: N=1
    dp[START][DIGIT_5] = 1;  // "5"
    dp[START][DIGIT_8] = 1;  // "8"
    
    // ДП переход
    for (int i = 2; i <= n; ++i) {
        // Очищаем новую таблицу
        for (int d1 = 0; d1 < 3; ++d1) {
            for (int d2 = 0; d2 < 3; ++d2) {
                new_dp[d1][d2] = 0;
            }
        }
        
        // Переходы
        for (int d1 = 0; d1 < 3; ++d1) {
            for (int d2 = 1; d2 < 3; ++d2) {  // d2 не может быть START
                if (dp[d1][d2] == 0) continue;
                
                // Попытка добавить 5
                if (!(d1 == DIGIT_5 && d2 == DIGIT_5)) {  // не три пятёрки подряд
                    new_dp[d2][DIGIT_5] += dp[d1][d2];
                }
                
                // Попытка добавить 8
                if (!(d1 == DIGIT_8 && d2 == DIGIT_8)) {  // не три восьмёрки подряд
                    new_dp[d2][DIGIT_8] += dp[d1][d2];
                }
            }
        }
        
        // Копирование для следующей итерации
        for (int d1 = 0; d1 < 3; ++d1) {
            for (int d2 = 0; d2 < 3; ++d2) {
                dp[d1][d2] = new_dp[d1][d2];
            }
        }
    }
    
    // Сумма всех завершённых последовательностей
    long long result = 0;
    for (int d1 = 0; d1 < 3; ++d1) {
        for (int d2 = 1; d2 < 3; ++d2) {
            result += dp[d1][d2];
        }
    }
    
    return result;
}
