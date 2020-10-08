#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <chrono>
#include "catch.hpp"
#include "function_calculator.h"
#include <iostream>


void testCalcFunc(const std::function<void(const Point &p)> &func) {
    for (int i = 0; i < 2; i++) {
        Point p{};
        p.x = i;
        p.y = i * 2;
        func(p);
    }
}

TEST_CASE("Test function calculator") {
    FunctionValuesCalculator calculator = FunctionValuesCalculator(testCalcFunc);
    std::stringstream write_ss;
    std::stringstream log_ss;
    calculator.Process(write_ss, log_ss);
    std::string expected_write = "0 0\n2 1\n";
    REQUIRE(expected_write == write_ss.str());
    REQUIRE(log_ss.str().find("point: y: 0, x: 0,") != std::string::npos);
    REQUIRE(log_ss.str().find("point: y: 2, x: 1,") != std::string::npos);
}