#pragma once

#include "Action.cpp"
#include "ActionResult.cpp"
using namespace std;

#define DELETE_FILE_ARGUMENTS_NUMBER 1

class DeleteFileAction : public Action {
public:

    DeleteFileAction(const vector<string> &arguments, const FileSystemService &service) : Action(arguments, service) {}

    ActionResult *execute() override {
        string path = getArguments().front();
        return getService().deleteFile(path);
    }

    int getArgumentsNumber() override {
        return DELETE_FILE_ARGUMENTS_NUMBER;
    }
};