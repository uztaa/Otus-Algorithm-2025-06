#include "TestAssert.h"

#include <chrono>

TestResult TestAssert::assert(const TestParam& testParam, const Counter& counter) const
{
	auto start = std::chrono::high_resolution_clock::now(); // Засекаем время

	uint64_t calcResult = counter.count(testParam.getIn());
	bool success = testParam.getOut() == calcResult;
	
	auto end = std::chrono::high_resolution_clock::now(); // Фиксируем завершение

	// Вычисляем длительность
	auto durationNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	uint64_t duration = durationNanos.count();

	return TestResult{
				testParam,
				calcResult,
				success,
				duration
	};
}
