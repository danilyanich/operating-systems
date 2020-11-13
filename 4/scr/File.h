//
// Created by anton on 12.11.2020.
//

#ifndef OS4_FILE_H
#define OS4_FILE_H

#include <string>
#include "Block.h"


class File {

private:
    int id;
     std::string name;
     Block *first;
public:
    File(int id, const std::string &name, Block *first);

    const std::string &getName() const;

    Block *getFirst() const;
};


#endif //OS4_FILE_H
