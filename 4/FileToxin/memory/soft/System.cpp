//
// Created by Petri on 10/13/2020.
//

#include "System.h"
#include <iostream>
#include <utility>

System::System(IndexesTable table) : table(std::move(table)) {
    this->totalFilesAmount = 0;
}

bool System::allocateFile(const string &name) {
    return allocateFile(name, totalFilesAmount);
}

bool System::allocateFile(const string &name, int id) {
    Index *firstFileIdx = this->table.allocateFile();
    if (firstFileIdx != nullptr &&
        files.find(name) == files.end()) {
        File *file = new File(
                id,
                name,
                firstFileIdx);
        files.insert(pair<string, File *>(name, file));
        this->totalFilesAmount++;
        return true;
    }
    return false;
}

string System::readFile(const string &path) const {
    if (files.find(path) != files.end()) {
        return table.readFile(files.find(path)->second->first);
    }
    return "";
}

void System::writeToFile(const string &path, char v) {
    if (files.find(path) != files.end()) {
        table.appendValueTo(files.find(path)->second->first, v);
    }
}

void System::copyFile(const string &from, const string &to) {
    if (files.find(from) != files.end() &&
        files.find(to) == files.end()) {
        Index *copy = table.copyFile(files.find(from)->second->first);
        if (copy != nullptr) {
            File *file = new File(
                    totalFilesAmount,
                    to,
                    copy);
            files.insert(pair<string, File *>(to, file));
            this->totalFilesAmount++;
        }
    }
}

void System::deleteFile(const string &path) {
    if (files.find(path) != files.end()) {
        IndexesTable::deleteFile(files.find(path)->second->first);
        files.erase(path);
        this->totalFilesAmount--;
    }
}

void System::moveFile(const string &from, const string &to) {
    copyFile(from, to);
    deleteFile(from);
}

void System::linkFIle(const string &file, const string &link) {
    if (files.find(file) != files.end() &&
        files.find(link) == files.end()) {
        this->files.
        insert(pair<string, File *>(
                link,
                files.find(file)->second));
    }
}

void System::createDump(const string &path) {

}

void System::loadDump(const string &path) {

}

void System::listAllFiles() {
    auto it = files.begin();
    for (it; it != files.end(); ++it)
        cout << it->first << '\n';
}

//in
istream &operator>>(istream &is, System &system) {
    int size, links;
    is >> links
       >> size;
    auto *memory = new PhysicalMemory(size);
    auto *table = new IndexesTable(*memory);
    system.table = *table;
    system.files.clear();
    system.totalFilesAmount = 0;
    set<int> usedIds;
    map<int, string> fileToPath;
    for (int i = 0; i < links; i++) {
        string link, content;
        int id;
        is >> link >> content >> id;
        if (usedIds.find(id) == usedIds.end()) {
            system.allocateFile(link);
            for (char v : content) {
                system.writeToFile(link, v);
            }
            usedIds.insert(id);
        } else {
            auto it = system.files.begin();
            for (; it != system.files.end(); ++it) {
                if (it->second->id == id) {
                    system.linkFIle(it->first, link);
                    break;
                }
            }
        }
    }
    return is;
}
//out
ostream &operator<<(ostream &os, System &system) {
    os << system.files.size() << " "
       << system.table.memory.getTotalMemory() << " ";
    auto it = system.files.begin();
    for (; it != system.files.end(); ++it) {
        os << it->first << " "
           << system.table.readFile(it->second->first) << " "
           << it->second->id << " ";
    }
    return os;
}
