//
// Created by illfate on 10/8/20.
//

#include <iostream>
#include "function_calculator.h"

class CommandParser {
    std::vector<std::string_view> tokens;
public:
    CommandParser(int argc, char **argv) {
        tokens.reserve(argc - 1);
        for (int i = 1; i < argc; ++i) {
            tokens.emplace_back(argv[i]);
        }
    }

    std::string_view GetFlagValue(std::string_view flag) {
        auto it = std::find(tokens.begin(), tokens.end(), flag);
        if (it == tokens.end()) {
            throw std::invalid_argument("can't find passed flag");
        }
        return *(++it);
    }
};

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