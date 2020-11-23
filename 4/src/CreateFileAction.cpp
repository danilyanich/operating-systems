#pragma once

#include "Action.cpp"
#include "ActionResult.cpp"
using namespace std;

#define CREATE_FILE_ARGUMENTS_NUMBER 1

class CreateFileAction : public Action {
public:

    CreateFileAction(const vector<string> &arguments, const FileSystemService &service) : Action(arguments, service) {}

    ActionResult *execute() override {
        string path = getArguments().front();
        return getService().createFile(path);
    }

    int getArgumentsNumber() override {
        return CREATE_FILE_ARGUMENTS_NUMBER;
    }
};