#pragma once

#include <exception>

using namespace std;

class ClusterNotFoundException : public exception {
public:
    const char *what() const noexcept override {
        return "Error of file";
    }
};