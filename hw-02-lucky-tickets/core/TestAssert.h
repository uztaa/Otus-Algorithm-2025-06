#pragma once

#include "TestParam.h"
#include "Counter.h"
#include "TestResult.h"

class TestAssert {
public:
	TestResult assert(const TestParam& testParam, const Counter& counter) const;
};

