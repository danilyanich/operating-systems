#pragma once
#include "file.h"
#include <iostream>
#include <list>

class FileSystem
{
private:
    std::list<File> files;
public:
    FileSystem();
    void createFile(std::string);
    void deleteFile(std::string);
    void readFile(std::string);
    void writeFile(std::string, std::string);
    File& copy(std::string);
    void showFiles();
    void showInfo(std::string);
};
