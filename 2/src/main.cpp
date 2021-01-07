
#include <iostream>
#include "function_calculator/function_calculator.h"
#include "function_calculator/command_parser.h"

int main(int argc, char **argv) {
    CommandParser parser(argc, argv);
    std::string_view file_name;
    try {
        file_name = parser.GetFlagValue("-file");
    }
    catch (const std::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return 1;
    }

    FunctionValuesCalculator calculator(functionValuesProducer);
    std::ofstream of(file_name.data());
    calculator.Process(of, std::cout);
}