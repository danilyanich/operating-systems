#pragma once

#include <map>

#include "FileExistException.cpp"
#include "FileNotFoundException.cpp"
#include "OutOfMemoryException.cpp"
#include "CustomFile.cpp"

#define FILE_NOT_FOUND 0
#define DEFAULT_TOTAL_MEMORY 1024

using namespace std;

class InterfaceFileSystem {
private:
    map<string, CustomFile *> *nameToFile = new map<string, CustomFile *>();

    unsigned long totalMemory;

    unsigned long capturedMemory = 0;

public:

    InterfaceFileSystem() {
        InterfaceFileSystem(DEFAULT_TOTAL_MEMORY);
    }

    InterfaceFileSystem(unsigned long totalMemory) {
        this->totalMemory = totalMemory;
    }

    vector<CustomFile *> *findAll() {
        vector<CustomFile *> *files = new vector<CustomFile *>();
        for (map<string, CustomFile *>::iterator it = nameToFile->begin(); it != nameToFile->end(); it++) {
            files->push_back(it->second);
        }
        return files;
    }

    CustomFile *findByName(const string &name) {
        if (nameToFile->count(name) > FILE_NOT_FOUND) {
            return nameToFile->find(name)->second;
        } else {
            throw FileNotFoundException();
        }
    }

    CustomFile *save(const string &name, CustomFile *file) {
        if (nameToFile->count(name) == FILE_NOT_FOUND) {
            nameToFile->insert(pair<string, CustomFile *>(name, file));
            decreaseFreeMemory(file->getSize());
            return file;
        } else {
            throw FileIsExistException();
        }
    }

    CustomFile *deleteFile(const string &name) {
        CustomFile *file = findByName(name);
        increaseFreeMemory(file->getSize());
        nameToFile->erase(name);
        return file;
    }

    void updateFile(const string &name, CustomFile *file) {
        if (nameToFile->count(name) > FILE_NOT_FOUND) {
            const unsigned long previousSize = nameToFile->find(name)->second->getSize();
            const unsigned long currentSize = file->getSize();
            increaseFreeMemory(previousSize);
            if (currentSize <= getFreeMemory()) {
                nameToFile->insert(pair<string, CustomFile *>(name, file));
                decreaseFreeMemory(currentSize);
            } else {
                decreaseFreeMemory(previousSize);
                throw OutOfMemoryException();
            }
        } else {
            throw FileNotFoundException();
        }
    }

private:
    void increaseFreeMemory(unsigned long memory) {
        capturedMemory -= memory;
    }

    void decreaseFreeMemory(unsigned long memory) {
        capturedMemory += memory;
    }

    unsigned long getFreeMemory() {
        return totalMemory - capturedMemory;
    }
};