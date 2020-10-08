#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <chrono>
#include "catch.hpp"
#include "function_calculator.h"
#include <iostream>


TEST_CASE("Test producer consumer") {
    FunctionValuesCalculator calculator = FunctionValuesCalculator(<#initializer#>, <#initializer#>);
    std::ofstream of("/home/illfate/CLionProjects/operating-systems/2/1.txt");
    calculator.Process(of, std::cout);
}