//
// Created by anton on 12.11.2020.
//

#include "Manager.h"
#include <iostream>

Manager::Manager(ControlM control) :control(std::move(control))
{
    this->fileCount = 0;
}

bool Manager::allocateFile(const string& name) {
    Block* begin = this->control.allocateFile();
    if (begin != nullptr &&
        files.find(name) == files.end()) {
        File* file = new File(
            fileCount,
            name,
            begin);
        files.insert(pair<string, File*>(name, file));
        this->fileCount++;
        return true;
    }
    return false;
}

string Manager::readFile(const string& name) const {
    if (files.find(name) != files.end()) {

        return  control.readFile(files.at(name)->getFirst());
    }
    return "";
}

void Manager::writeToFile(const string& name, char v) {
    if (files.find(name) != files.end()) {
        control.appendValueTo(files.at(name)->getFirst(), v);
    }
}


void Manager::copyFile(const string& from, const string& to) {
    if (files.find(from) != files.end() && files.find(to) == files.end()) {
        Block* copy = control.copyFile(files.at(from)->getFirst());
        if (copy != nullptr) {
            File* file = new File(
                fileCount,
                to,
                copy);
            files.insert(pair<string, File*>(to, file));
            this->fileCount++;
        }
    }
}



void Manager::deleteFile(const string& name) {
    if (files.find(name) != files.end()) {
        control.deleteFile(files.at(name)->getFirst());
        files.erase(name);
        fileCount--;
    }

}

void Manager::moveFile(const string& from, const string& to) {
    copyFile(from, to);
    deleteFile(from);
}

void Manager::listAllFiles() {
    auto it = files.begin();
    for (it; it != files.end(); ++it)
        cout << it->first << '\n';
}