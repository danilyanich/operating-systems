cmake_minimum_required(VERSION 3.15)
project(2)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(2 src/function_calculator/consumer_producer.h src/main.cpp src/function_calculator/function_calculator.h src/function_calculator/command_parser.h)
add_executable(tests src/function_calculator/consumer_producer.h src/tests/test.cpp)

enable_testing()
add_test(NAME tests COMMAND test_suite1)
