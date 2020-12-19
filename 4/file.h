#pragma once
#include <string>
#include <chrono>
#include <ctime>

class File
{
private:
    std::string name;
    std::string content;
    std::string author;
    std::time_t date;
    int size;
public:
    File();
    File(std::string);
    void setFileName(std::string);
    void setFileContent(std::string);
    std::string getName();
    char* getDate();
    std::string getContent();
    int getSize();
};
