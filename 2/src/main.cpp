
#include <iostream>
#include "function_calculator/function_calculator.h"

int main(int argc, char **argv) {
    std::string_view file_name;
    file_name = "1.txt";

    FunctionValuesCalculator calculator(functionValuesProducer);
    std::ofstream of(file_name.data());
    calculator.Process(of, std::cout);
}