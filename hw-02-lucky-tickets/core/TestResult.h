#pragma once

#include "TestParam.h"

struct TestResult {
	const TestParam& testParam;
	uint64_t calcResult;
	bool success;
	uint64_t duration;
};