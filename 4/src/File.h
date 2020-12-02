//
// Created by Makarevich on 25/12/2020.
//
#pragma once

#ifndef OS4_FILE_H
#define OS4_FILE_H


#include <string>
#include <utility>
#include "Table.h"

class File
{
public:
    explicit File(std::string  name) :
        name(std::move(name))
    {
        index = Table::createChunk();
    }
    int index;
    std::string name;
    void write(char* buf)const;
    std::string read()const;
};


#endif //OS4_FILE_H
