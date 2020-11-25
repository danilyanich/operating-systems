#ifndef INC_2_PARSER_H
#define INC_2_PARSER_H

#include <vector>
#include <algorithm>

using namespace std;

class Parser {
    
    vector <string> parts;

public:

    Parser(int argc, char** argv);

    string getFlagValue(string flag);

};


Parser::Parser(int argc, char** argv) {
    
    parts.reserve(argc - 1);
    for (int i = 1; i < argc; i++) {
        parts.push_back(argv[i]);
    }

}

string Parser::getFlagValue(string flag) {

    auto it = find(parts.begin(), parts.end(), flag);
    if (it == parts.end()) {
        throw invalid_argument("Flag Error");
    }

    return *(++it);
}

#endif