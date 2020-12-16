#pragma once

#include "Action.cpp"
#include "ActionResult.cpp"

using namespace std;

#define LIST_FILES_ARGUMENTS_NUMBER 0

class ListFilesAction : public Action {
public:

    ListFilesAction(const vector<string> &arguments, const FileSystemService &service) : Action(arguments, service) {}

    ActionResult *execute() override {
        return getService().listFiles();
    }

    int getArgumentsNumber() override {
        return LIST_FILES_ARGUMENTS_NUMBER;
    }
};

