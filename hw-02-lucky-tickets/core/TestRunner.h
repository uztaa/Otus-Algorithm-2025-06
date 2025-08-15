#pragma once

#include <string>
#include "Counter.h"

class TestRunner {
public:
	/* Запуск тестов. */
	// counter - объект тестирования
	// dir - папка с параметрами тестов (файлы in, out)
	// n - количество загружаемых тестов
	void runTests(const Counter& counter, const std::string& dir, int n);
};