#pragma once

#include "Action.cpp"
#include "ActionResult.cpp"
using namespace std;

#define WRITE_FILE_ARGUMENTS_NUMBER 2
#define OFFSET 1

class RewriteFileAction : public Action {
public:

    RewriteFileAction(const vector <string> &arguments, const FileSystemService &service) : Action(arguments,
                                                                                                   service) {}

    ActionResult *execute() override {
        string path = getArguments().front();
        int size = 0;
        char *data = extractDataFromArguments(&size);
        return getService().rewriteFile(path, data, size);
    }

    int getArgumentsNumber() override {
        return WRITE_FILE_ARGUMENTS_NUMBER;
    }

    bool isMultiArguments() override {
        return true;
    }

private:
    char *extractDataFromArguments(int *size) {
        for (int index = OFFSET; index < getArguments().size(); index++) {
            *size += getArguments().at(index).size();
        }
        char *data = new char[*size];
        char *buffer = data;
        for (int index = OFFSET; index < getArguments().size(); index++) {
            int sizeOfText = getArguments().at(index).size();
            memcpy(buffer, getArguments().at(index).c_str(), sizeOfText);
            buffer += sizeOfText;
        }
        return data;
    }
};