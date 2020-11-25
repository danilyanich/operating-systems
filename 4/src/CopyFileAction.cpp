#pragma once

#include "ActionResult.cpp"
#include "Action.cpp"
using namespace std;

#define COPY_FILE_ARGUMENTS_NUMBER 2

class CopyFileAction : public Action {
public:

    CopyFileAction(const vector<string> &arguments, const FileSystemService &service) : Action(arguments, service) {}

    ActionResult *execute() override {
        string path = getArguments().front();
        string copyPath = getArguments().back();
        return getService().copyFile(path, copyPath);
    }

    int getArgumentsNumber() override {
        return COPY_FILE_ARGUMENTS_NUMBER;
    }
};
