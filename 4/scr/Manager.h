//
// Created by anton on 12.11.2020.
//

#ifndef OS4_MANAGER_H
#define OS4_MANAGER_H

#include "ControlM.h"
#include "File.h"
#include <string>
#include <map>
using namespace std;

class Manager {
public:
    int fileCount;
    ControlM control;
    map<string, File*> files;

    explicit Manager(ControlM control);
    bool allocateFile(const string& name);
    string readFile(const string& path) const;
    void writeToFile(const string& path, char v);
    void copyFile(const string& from, const string& to);
    void deleteFile(const string& path);
    void moveFile(const string& from, const string& to);
    void listAllFiles();
};


#endif //OS4_MANAGER_H
