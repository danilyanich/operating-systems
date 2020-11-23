#pragma once

using namespace std;

#include "Action.cpp"
#include "ActionResult.cpp"

#define DUMP_ARGUMENTS_NUMBER 0

class DumpAction : public Action {
public:

    DumpAction(const vector<string> &arguments, const FileSystemService &service) : Action(arguments, service) {}

    ActionResult *execute() override {
        return getService().dump();
    }

    int getArgumentsNumber() override {
        return DUMP_ARGUMENTS_NUMBER;
    }
};