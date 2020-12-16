#pragma once

#include <exception>

using namespace std;

class FileIsExistException : public exception {
public:
    const char *what() const noexcept override {
        return "File already exist";
    }
};