#pragma once

#include <exception>

using namespace std;

class OutOfMemoryException : public exception {
public:
    const char *what() const noexcept override {
        return "Out of memory";
    }
};