#pragma once

#include "Action.cpp"
#include "ActionResult.cpp"
using namespace std;

#define READ_FILE_ARGUMENTS_NUMBER 1

class ReadFileAction : public Action {
public:

    ReadFileAction(const vector<string> &arguments, const FileSystemService &service) : Action(arguments, service) {}

    ActionResult *execute() override {
        string path = getArguments().front();
        return getService().readFile(path);
    }

    int getArgumentsNumber() override {
        return READ_FILE_ARGUMENTS_NUMBER;
    }
};