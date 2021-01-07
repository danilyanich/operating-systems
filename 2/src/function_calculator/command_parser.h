

#ifndef INC_2_COMMAND_PARSER_H
#define INC_2_COMMAND_PARSER_H

#include <vector>
#include <string_view>
#include <algorithm>

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

#endif //INC_2_COMMAND_PARSER_H
