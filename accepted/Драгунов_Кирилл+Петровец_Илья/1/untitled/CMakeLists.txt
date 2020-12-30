cmake_minimum_required(VERSION 3.16)
project(untitled C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(UNIT_TESTS_FILES src/mmemory.c tests/unit_tests.c header/memory_model.h header/util.h)
set(LOAD_TESTS_FILES src/mmemory.c tests/load_tests.c header/memory_model.h header/util.h)
add_executable(unit ${UNIT_TESTS_FILES})
add_executable(load ${LOAD_TESTS_FILES})