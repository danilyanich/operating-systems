//
// Created by Petri on 10/13/2020.
//

#include "File.h"

#include <utility>

const string &File::getName() const {
    return name;
}

Index *File::getFirst() const {
    return first;
}

File::File(int id,
        string name,Index *first) :
        id(id),
        name(std::move(name)),
        first(first) {}
