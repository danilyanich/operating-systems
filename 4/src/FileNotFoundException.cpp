#pragma once

#include <exception>

using namespace std;

class FileNotFoundException : public exception {
public:
    const char *what() const noexcept override {
        return "File not found";
    }
};