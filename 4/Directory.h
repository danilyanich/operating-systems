//
// Created by Anton Schur on 12/12/2020
//

#ifndef OS4_DIRECTORY_H
#define OS4_DIRECTORY_H


#include <utility>
#include <vector>
#include <string>
#include "File.h"

class Directory
{
public:
    std::string name;
    Directory() = default;
    explicit Directory(std::string  name) :
        name(std::move(name))
    {

    }
    std::shared_ptr<Directory> & findDirectory(const std::string& dirName);
    std::shared_ptr<File> & findFile(const std::string& fileName);
    void addFile(const std::string& file);
    void moveFile(std::shared_ptr<File>& file);
    void delFile(std::shared_ptr<File> &file);
    void addDir(const std::string& dir);
    void delDir(std::shared_ptr<Directory>& dir);
    void print();
private:
    std::vector<std::shared_ptr<Directory>> children{};
    std::vector<std::shared_ptr<File>> files{};
};


#endif //OS4_DIRECTORY_H
