#pragma once

#include <vector>
#include <sstream>

#include "FileSystemService.cpp"
#include "IncorrectInputException.cpp"

#include "Action.cpp"
#include "CreateFileAction.cpp"
#include "DeleteFileAction.cpp"
#include "CopyFileAction.cpp"
#include "MoveFileAction.cpp"
#include "ReadFileAction.cpp"
#include "WriteFileAction.cpp"
#include "RewriteFileAction.cpp"
#include "DumpAction.cpp"
#include "ListFilesAction.cpp"

#define ARGUMENTS_OFFSET 1

#define CREATE_FILE_ACTION "create"
#define DELETE_FILE_ACTION "delete"
#define COPY_FILE_ACTION "copy"
#define MOVE_FILE_ACTION "move"
#define READ_FILE_ACTION "read"
#define WRITE_FILE_ACTION "write"
#define REWRITE_FILE_ACTION "rewrite"
#define DUMP_ACTION "dump"
#define LIST_FILES_ACTION "ls"

using namespace std;

class ExpressionResolver {

private:
    FileSystemService *service;

public:
    explicit ExpressionResolver(FileSystemService *service) {
        this->service = service;
    }

    Action *resolve(const string &expression) {
        vector<string> tokens;
        split(expression, &tokens);
        string actionType = getType(&tokens);
        vector<string> arguments;
        getArguments(&tokens, &arguments);
        return validate(build(actionType, &arguments));
    }

private:
    void split(const string &source, vector<string> *result) {
        stringstream stream(source);
        string substring;
        char delimiter = ' ';
        while (getline(stream, substring, delimiter)) {
            result->push_back(substring);
        }
    }

    string &getType(vector<string> *tokens) {
        if (!tokens->empty()) {
            return tokens->front();
        } else {
            throw IncorrectInputException();
        }
    }

    void getArguments(vector<string> *tokens, vector<string> *arguments) {
        if (tokens->size() > ARGUMENTS_OFFSET) {
            *arguments = vector<string>(tokens->begin() + ARGUMENTS_OFFSET, tokens->end());
        }
    }

    Action *build(const string &type, vector<string> *arguments) {
        if (type == CREATE_FILE_ACTION) return new CreateFileAction(*arguments, *service);
        if (type == DELETE_FILE_ACTION) return new DeleteFileAction(*arguments, *service);
        if (type == COPY_FILE_ACTION) return new CopyFileAction(*arguments, *service);
        if (type == MOVE_FILE_ACTION) return new MoveFileAction(*arguments, *service);
        if (type == READ_FILE_ACTION) return new ReadFileAction(*arguments, *service);
        if (type == WRITE_FILE_ACTION) return new WriteFileAction(*arguments, *service);
        if (type == REWRITE_FILE_ACTION) return new RewriteFileAction(*arguments, *service);
        if (type == DUMP_ACTION) return new DumpAction(*arguments, *service);
        if (type == LIST_FILES_ACTION) return new ListFilesAction(*arguments, *service);
        throw IncorrectInputException();
    }

    Action *validate(Action *action) {
        if (action->getArguments().size() == action->getArgumentsNumber()) {
            return action;
        }
        if (action->isMultiArguments() && action->getArgumentsNumber() <= action->getArguments().size()) {
            return action;
        }
        throw IncorrectInputException();
    }
};