#include <iostream>
#include <string>
#include <Windows.h>
#include "FileSystemController.cpp"

int main(){
    FileSystemController* fileSystemController = new FileSystemController(200, 4);
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    while (true) {
        SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
        std::string command;
        std::cout << fileSystemController->getPath();
        std::getline(std::cin, command);
        if (command == "exit") break;
        fileSystemController->action(command);
    }
}