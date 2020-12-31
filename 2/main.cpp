#include <iostream>
#include "Calculator.h"
#include "Parser.h"

using namespace std;

void testCalc(const std::function<void(const Point & p)>& func) {
    for (int i = 0; i < 2; i++) {
        Point p{};
        p.x = i;
        p.y = i * 2;
        func(p);
    }
}

int main(int argc, char** argv) {
    
    Parser parser(argc, argv);
    
    string file_name;
    cin >> file_name;

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