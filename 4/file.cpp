#include "file.h"
#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdlib.h>

File::File() {
    name = "unnamed_file";
    content = "\n";
    size = 0;
    date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
};

File::File(std::string file_name){
    name = file_name; 
    content = "";
    size = 0;
    date = time(0);
};

void File::setFileName(std::string file_name){
    name = file_name;
}

void File::setFileContent(std::string file_content){
    content = file_content;
    size = content.size();
}

std::string File::getName(){
    return name;
}

char* File::getDate(){
    return std::ctime(&date);
}

std::string File::getContent(){
    return content;
}

int File::getSize(){
    return size;
}
