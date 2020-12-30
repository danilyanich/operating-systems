#include "fileSystem.h"


FileSystem::FileSystem(){
};

void FileSystem::createFile(std::string file_name){
    File new_file(file_name);
    files.push_back(new_file);
};

void FileSystem::deleteFile(std::string file_name){
    for (std::list<File>::iterator it=files.begin(); it!=files.end(); ++it)
        if((*it).getName() == file_name) files.erase(it);
};

void FileSystem::readFile(std::string file_name){
    for (std::list<File>::iterator it=files.begin(); it!=files.end(); ++it)
        if((*it).getName() == file_name) std::cout<<(*it).getContent()<<std::endl;
};

void FileSystem::writeFile(std::string file_name, std::string new_content){
     for (std::list<File>::iterator it=files.begin(); it!=files.end(); ++it)
        if((*it).getName() == file_name){
            (*it).setFileContent(new_content);
        }
};

File& FileSystem::copy(std::string file_name){
     for (std::list<File>::iterator it=files.begin(); it!=files.end(); ++it)
        if((*it).getName() == file_name) return (*it);
};

void FileSystem::showFiles(){
     for (std::list<File>::iterator it=files.begin(); it!=files.end(); ++it)
        std::cout<<(*it).getName()<<std::endl;
};

void FileSystem::showInfo(std::string file_name){
     for (std::list<File>::iterator it=files.begin(); it!=files.end(); ++it)
        if((*it).getName() == file_name){
            std::cout<<"Name:"<<(*it).getName()<<std::endl;
            std::cout<<"Date:"<<(*it).getDate();
            std::cout<<"Size:"<<(*it).getSize()<<std::endl;
            std::cout<<"Content:"<<(*it).getContent()<<std::endl;
        }
};
