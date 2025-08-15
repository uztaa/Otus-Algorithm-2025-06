add_executable (hw-02-lucky-tickets "main.cpp" "core/FileReader.h" "core/ FileReader.cpp" "LuckyTicketCounter.cpp" "core/TestAssert.h" "core/TestParam.cpp" "core/TestParamLoader.cpp"  "core/TestAssert.cpp" "core/TestRunner.h" "core/TestRunner.cpp"  "core/Counter.h" "core/TestResult.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET hw-02-lucky-tickets PROPERTY CXX_STANDARD 17)
endif()
