#include "TestParam.h"

TestParam::TestParam(const std::string& name, int in, int64_t out) :
	name{ name }, in{ in }, out{ out } {
};

// getters
std::string TestParam::getName() const { return name; }
int TestParam::getIn() const { return in; }
int64_t TestParam::getOut() const { return out; }
