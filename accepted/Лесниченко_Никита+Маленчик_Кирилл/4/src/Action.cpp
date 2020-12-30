#pragma once

#include <vector>
#include "ActionResult.cpp"
#include "FileSystemService.cpp"

using namespace std;

class Action {
private:
    vector<string> arguments;

    FileSystemService service;

public:

    Action(const vector<string> &arguments, const FileSystemService &service) {
        this->arguments = arguments;
        this->service = service;
    }

    virtual ~Action() {
        arguments.clear();
    }

    vector<string> &getArguments() {
        return arguments;
    }

    FileSystemService &getService() {
        return service;
    }

    virtual bool isMultiArguments() {
        return false;
    }

    virtual ActionResult *execute() = 0;

    virtual int getArgumentsNumber() = 0;
};