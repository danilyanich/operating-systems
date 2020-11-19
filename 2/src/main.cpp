#include <iostream>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <string>

#include "Point.h"

std::fstream result_file;

void init_file(char filename[]) {
    result_file.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
    result_file.close();
}

void record_to_file(char filename[], std::string result) {
    result_file.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
    result_file << result << "\n";
    result_file.close();
}


int main() {
   
    return 0;
}
