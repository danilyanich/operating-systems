//
// Created by illfate on 10/8/20.
//

#include <iostream>
#include "function_calculator.h"


int main() {
    FunctionValuesCalculator calculator(functionValuesProducer);
    std::ofstream of("/home/illfate/CLionProjects/operating-systems/2/1.txt");
    calculator.Process(of, std::cout);
}