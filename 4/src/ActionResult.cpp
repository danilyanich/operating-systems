#pragma once

#include <cstring>
#include <string>
using namespace std;

class ActionResult {
private:
    string message;

    bool successful;
public:

    ActionResult(const string &message, bool successful) {
        this->message = message;
        this->successful = successful;
    }

    const string &getMessage() const {
        return message;
    }

    bool isSuccessful() const {
        return successful;
    }
};