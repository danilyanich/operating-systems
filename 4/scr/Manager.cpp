#include "Manager.h"
#include <set>
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

ostream& operator<<(ostream& os, Manager& manager) {
    os << manager.files.size() << "  "
        << manager.control.memory.getSizeOfMemory() << "  ";
    auto it = manager.files.begin();
    
    for (; it != manager.files.end(); ++it) {
        os << it->first << " "
            << manager.control.readFile(it->second->getFirst()) << " "
            << it->second->getId()<< " ";
    }
    return os;
}

void Manager::linkFIle(const string& file, const string& link) {
    if (files.find(file) != files.end() &&
        files.find(link) == files.end()) {
        this->files.
            insert(pair<string, File*>(
                link,
                files.find(file)->second));
    }
}

istream& operator>>(istream& is, Manager& manager) {
    int size, links;
    is >> links >> size;
    auto* memory = new PhMemory(size);
    auto* control = new ControlM(*memory);
    manager.control = *control;
    manager.files.clear();
    manager.fileCount = 0;
    set<int> usedIds;
    map<int, string> fileToPath;
    for (int i = 0; i < links; i++) {
        string link, content;
        int id;
        is >> link >> content >> id;
        if (usedIds.find(id) == usedIds.end()) {
            manager.allocateFile(link);
            for (char v : content) {
                manager.writeToFile(link, v);
            }
            usedIds.insert(id);
        }
        else {
            auto it = manager.files.begin();
            for (; it != manager.files.end(); ++it) {
                if (it->second->getId() == id) {
                    manager.linkFIle(it->first, link);
                    break;
                }
            }
        }
    }
    return is;
}