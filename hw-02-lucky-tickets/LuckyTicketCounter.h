#pragma once

#include <vector>

#include "core/Counter.h"

class LuckyTicketCounter : public Counter {
public:

	/* Считает количество счастливых 2n-билетов */
	int64_t count(int n) const override;

	/* Функция вычисляет сумму квадратов элементов вектора */
	int64_t sumOfSquares(const std::vector<int64_t>& vec) const;

	/* Создание следующего вектора из текущего */
	std::vector<int64_t> nextVector(std::vector<int64_t> inVector) const;
	
	/* Суммирует элементы по столбцам матрицы */
	std::vector<int64_t> sumColumns(const std::vector<std::vector<int64_t>>& matrix) const;
	
	/* Функция для вывода матрицы на экран */
	void printMatrix(const std::vector<std::vector<int64_t>>& matrix) const;
	
	/* Добавляет zerosLeft слева и zerosRight справа к вектору */
	std::vector<int64_t> addPadding(const std::vector<int64_t>& input, int zerosLeft, int zerosRight) const;
	
	/* Создание матрицы из ветора со сдвигом */
	std::vector<std::vector<int64_t>> createMatrixWithPadding(const std::vector<int64_t>& input) const;
};