//
// Created by Petri on 10/13/2020.
//

#ifndef FILESYSTEM_SYSTEM_H
#define FILESYSTEM_SYSTEM_H

#include "../hard/IndexesTable.h"
#include "File.h"
#include <map>
#include <istream>
#include <set>

class System {
public:
    int totalFilesAmount{};
    IndexesTable table;
    map<string, File*> files;
    explicit System(IndexesTable table);
    bool allocateFile(const string& name);
    string readFile(const string& path) const ;
    void writeToFile(const string& path, char v);
    void copyFile(const string& from, const string& to);
    void deleteFile(const string& path);
    void moveFile(const string& from, const string& to);
    void linkFIle(const string& file, const string& link);
    void createDump(const string& path);
    void loadDump(const string& path);
    void listAllFiles();
    friend istream& operator>>(istream &is, System& system);
    friend ostream& operator<<(ostream& os, System& system);

    bool allocateFile(const string &name, int id);
};


#endif //FILESYSTEM_SYSTEM_H
