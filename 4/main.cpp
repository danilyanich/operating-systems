#include <iostream>
#include "structHeader.h"

struct filesystem fileSystem;

void runCommand(string command) {

    if (!regex_match(command, regex(DATA_INP_REGEXP))) {
        cout << "Error! Wrong Command!\n";
        return;
    }

    int index = 0;
    while (index < command.length() && command[index] != ' ') {
        index++;
    }

    string message, textCommand = command.substr(0, index);

    if (index != command.length()) {
        message = command.substr(index + 1, command.length() - 1);
    }

    string s = fileSystem.fileSystemName;
    if (s.compare("") == 0 && textCommand != "init" && textCommand != "load" && textCommand != "exit") {
        cout << "Filesystem hasn't loaded\n";
        return;
    }

    if (textCommand == "init") {
        init(message);
    }
    else if (textCommand == "load") {
        fileSystem.fileSystemName = load(message, fileSystem);
    }
    else if (textCommand == "create") {
        create(message, fileSystem);
    }
    else if (textCommand == "remove") {
        remove(message, fileSystem);
    }
    else if (textCommand == "copy") {
        copy(message, fileSystem);
    }
    else if (textCommand == "move") {
        move(message, fileSystem);
    }
    else if (textCommand == "write") {
        write(message, fileSystem);
    }
    else if (textCommand == "read") {
        read(message, fileSystem);
    }
    else if (textCommand == "ls") {
        ls(fileSystem);
    }
    else if (textCommand == "exit") {
        exit(0);
    }
    else {
        cout << "Error! Wrong Command!\n";
    }
}

int main() {
    while(true) {

        char command[1000];
        cout << "Command: ";
        
        cin.clear();
        cin.getline(command, 1000);
        
        cin.clear();
        runCommand(command);
    }
}