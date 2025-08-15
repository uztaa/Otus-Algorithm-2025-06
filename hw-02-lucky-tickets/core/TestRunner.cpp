#include "TestRunner.h"
#include "TestParamLoader.h"
#include "TestAssert.h"

#include "Counter.h"

void TestRunner::runTests(const Counter& counter, const std::string& dir, int n)
{
	TestAssert testAssert;
	
	TestParamLoader loader;
	std::vector<TestParam> params = loader.loadTestParams(dir, n);

	for (TestParam param : params) {

		TestResult testResult = testAssert.assert(param, counter);

		std::cout << "############" << std::endl;

		std::cout << "test name = " << testResult.testParam.getName() << std::endl;
		std::cout << "in = " << std::to_string(testResult.testParam.getIn()) << std::endl;
		std::cout << "out = " << std::to_string(testResult.testParam.getOut()) << std::endl;
		std::cout << "calcResult = " << std::to_string(testResult.calcResult) << std::endl;
		std::cout << "success = " << std::to_string(testResult.success) << std::endl;
		std::cout << "duration: " << std::to_string(testResult.duration) << " nanoseconds" << std::endl;

	}
}

