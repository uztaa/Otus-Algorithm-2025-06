function(auto_generate_tests)

set(options)
set(oneValueArgs
DATA_DIR
CLASS_NAME
METHOD_NAME
OUTPUT_FILE
STRONG_ASSERT
)
cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

if (NOT ARG_DATA_DIR OR NOT ARG_CLASS_NAME OR NOT ARG_METHOD_NAME OR NOT ARG_OUTPUT_FILE)
    message(FATAL_ERROR "Missing required arguments to auto_generate_tests")
endif()

set(GENERATED_FILE "${ARG_OUTPUT_FILE}")

file(WRITE "${GENERATED_FILE}" "#include <gtest/gtest.h>\n")
file(APPEND "${GENERATED_FILE}" "#include \"MathSettings.h\"\n")
file(APPEND "${GENERATED_FILE}" "#include \"${ARG_CLASS_NAME}.h\"\n\n")

set(INDEX 0)
while(TRUE)
    set(INPUT_FILE "${ARG_DATA_DIR}/test.${INDEX}.in")
    set(OUTPUT_FILE "${ARG_DATA_DIR}/test.${INDEX}.out")

    if (EXISTS "${INPUT_FILE}" AND EXISTS "${OUTPUT_FILE}")
        file(READ "${INPUT_FILE}" RAW_INPUT_CONTENT)
        string(REGEX REPLACE "\n" ";" INPUT_VALUES "${RAW_INPUT_CONTENT}")
        list(FILTER INPUT_VALUES EXCLUDE REGEX "^$")

        file(READ "${OUTPUT_FILE}" EXPECTED_RESULT)
        string(STRIP "${EXPECTED_RESULT}" "${EXPECTED_RESULT}")

        set(PARAMS_STR "")
        foreach(V ${INPUT_VALUES})
            string(STRIP "${V}" V_STRIPPED)
            set(PARAMS_STR "${PARAMS_STR}\"${V_STRIPPED}\", ")
        endforeach()

        string(REGEX REPLACE ", $" "" PARAMS_STR "${PARAMS_STR}")

        file(APPEND "${GENERATED_FILE}" "TEST(${ARG_CLASS_NAME}Test, Test${INDEX}) {\n")
        file(APPEND "${GENERATED_FILE}" "    ${ARG_CLASS_NAME} service;\n")

        if ("${ARG_STRONG_ASSERT}")
        file(APPEND "${GENERATED_FILE}" "    ASSERT_EQ(service.${ARG_METHOD_NAME}(${PARAMS_STR}), \"${EXPECTED_RESULT}\");\n")
        else()
        file(APPEND "${GENERATED_FILE}" "    EXPECT_NEAR(service.${ARG_METHOD_NAME}(${PARAMS_STR}), ${EXPECTED_RESULT}, EPS);\n")
        endif()

        file(APPEND "${GENERATED_FILE}" "}\n\n")

        math(EXPR INDEX "${INDEX} + 1")
    else()
        break()
    endif()
endwhile()

message(STATUS "Generated ${INDEX} test(s) in ${GENERATED_FILE}")

endfunction()