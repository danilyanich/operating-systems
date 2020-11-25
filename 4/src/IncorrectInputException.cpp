#pragma once

#include <exception>

using namespace std;

class IncorrectInputException : public exception {
public:
    const char* what() const noexcept override {
        return "Input is incorrect";
    }
};