#include "TestParamLoader.h"
#include "FileReader.h"

std::vector <TestParam> TestParamLoader::loadTestParams(const std::string& dir, int n)
{
	std::vector <TestParam> testParams;
	FileReader fileReader;

	for (int i = 0; i < n; ++i) {
		std::string basePath = dir + "\\test." + std::to_string(i);
		std::string inStr = fileReader.readFile(basePath + ".in");
		std::string outStr = fileReader.readFile(basePath + ".out");

		int in = std::stoi(inStr);
		int64_t out = std::stoll(outStr);

		testParams.push_back(TestParam{"test-" + std::to_string(i), in, out});
	}

	return testParams;
}
