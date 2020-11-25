#include <iostream>
#include "Calc.h"
#include "Parser.h"

using namespace std;

int main(int argc, char** argv) {
    
    Parser parser(argc, argv);
    string file_name;

    try {
        file_name = parser.getFlagValue("-file");
    }
    catch (const exception & exc) {
        cerr << exc.what() << endl;
        return 1;
    }

    Calculator calculator(produceFuncValues);
    ofstream fout(file_name.data());
    
    calculator.process(fout, cout);
}