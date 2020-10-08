cmake_minimum_required(VERSION 3.15)
project(2)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(2 consumer_producer.h main.cpp function_calculator.h command_parser.h)
add_executable(tests consumer_producer.h test.cpp)

enable_testing()
add_test(NAME tests COMMAND test_suite1)
