#include "command.h"

struct filesystem fs;

void startCommand(string command) {
    if (!regex_match(command, regex(DATA_INP_REGEXP))) {
        cout<<"wrong command\n";
        return;
    }

    int index = 0;
    while (index < command.length() && command[index] != ' ') {
        index++;
    }

    string commandType = command.substr(0, index);
    string message;
    if (index != command.length()) {
        message = command.substr(index + 1, command.length() - 1);
    }

    int key = -1;

    if (commandType == "init") key = 0;
    if (commandType == "load") key = 1;
    if (commandType == "create") key = 2;
    if (commandType == "remove") key = 3;
    if (commandType == "copy") key = 4;
    if (commandType == "move") key = 5;
    if (commandType == "write") key = 6;
    if (commandType == "read") key = 7;
    if (commandType == "ls") key = 9;
    if (commandType == "exit") key = 8;

    commandFactory(key, message);
}

void commandFactory(int key, string message) {
    if (!isFileSystemLoad() && key != 0 && key != 1 && key != 8) {
        cout<<"file system hasn't loaded\n";
        return;
    }

    switch (key) {
        case 0:
            init(message);
            break;
        case 1:
            fs.fileSystemName = load(message, fs);
            break;
        case 2:
            create(message, fs);
            break;
        case 3:
            remove(message, fs);
            break;
        case 4:
            copy(message, fs);
            break;
        case 5:
            move(message, fs);
            break;
        case 6:
            write(message, fs);
            break;
        case 7:
            read(message, fs);
            break;
        case 9:
            ls(fs);
            break;
        case 8:
            exit(0);
        default:
            cout<<"wrong command\n";
            break;
    }
}

bool isFileSystemLoad() {
    string s = fs.fileSystemName;
    if (!s.compare("") == 0) {
        return true;
    } else {
        return false;
    }
}