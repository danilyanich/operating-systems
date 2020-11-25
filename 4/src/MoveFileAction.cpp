#pragma once

#include "Action.cpp"
#include "ActionResult.cpp"
using namespace std;

#define MOVE_FILE_ARGUMENTS_NUMBER 2

class MoveFileAction : public Action {
public:

    MoveFileAction(const vector<string> &arguments, const FileSystemService &service) : Action(arguments, service) {}

    ActionResult *execute() override {
        string path = getArguments().front();
        string movePath = getArguments().back();
        return getService().moveFile(path, movePath);
    }

    int getArgumentsNumber() override {
        return MOVE_FILE_ARGUMENTS_NUMBER;
    }
};