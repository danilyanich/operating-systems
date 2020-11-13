//
// Created by anton on 12.11.2020.
//

#include "File.h"

File::File(int id, const std::string &name, Block *first) : id(id), name(name), first(first) {}

const std::string &File::getName() const {
    return name;
}

Block *File::getFirst() const {
    return first;
}
