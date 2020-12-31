//
// Created by Anton Schur on 12/12/2020
//
#pragma once

#ifndef OS4_FILESYSTEM_H
#define OS4_FILESYSTEM_H


#include "Directory.h"

class FileSystem
{
public:
    FileSystem();
    void run();

private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> current;
};


#endif //OS4_FILESYSTEM_H
