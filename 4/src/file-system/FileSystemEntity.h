//
// Created by illfate on 11/26/20.
//

#ifndef FILE_SYSTEM_FILESYSTEMENTITY_H
#define FILE_SYSTEM_FILESYSTEMENTITY_H


#include <string>

enum class FileSystemEntities {
    DIRECTORY,
    FILE
};

class FileSystemEntity {
public:
    virtual std::string GetName() const = 0;

    virtual FileSystemEntities GetType() const = 0;

    virtual void Delete() = 0;
};


#endif //FILE_SYSTEM_FILESYSTEMENTITY_H
