//
// Created by anton on 12.11.2020.
//

#include "ControlM.h"

ControlM::ControlM(PhMemory memory) : memory(memory) {

}

Block* ControlM::allocateFile() {
    Block* begin = findFreeIdx();

    return begin;
}

Block* ControlM::findFreeIdx() {
    for (int i = 0; i < memory.getSizeOfMemory(); i++) {
        if (memory.getValueAt(i)->id == -1) {
            memory.allocateValue(i, '\0');
            //printf("%d\n", i);
            return memory.getValueAt(i);
        }
    }
    return nullptr;
}

bool ControlM::appendValueTo(Block* first, char v) {

    
    if (memory.getValueAt(first->id)->cont== '\0') {
        memory.allocateValue(first->id, v);
        return  true;
    }
    Block* next = findFreeIdx();
    if (next != nullptr) {
        first->next = next;
        memory.allocateValue(next->id, v);
        return true;
    }
    return false;
}

string ControlM::readFile(Block* begin) const {

    string result;
    Block* buff = begin;
    do {
        result.insert(result.end(), buff->cont);
        buff = buff->next;
    } while (buff != nullptr);
    return result;
}

Block* ControlM::copyFile(Block* begin) {
    string result = readFile(begin);
    Block* newBegin = findFreeIdx();
    if (newBegin != nullptr) {

        for (auto re : result) {
            appendValueTo(newBegin, re);
        }
    }
    return newBegin;
}

void ControlM::deleteFile(Block* begin) {
    while (begin != nullptr)
    {
        begin->id = -1;
        Block* buff = begin;
        begin = begin->next;
        buff->next = nullptr;
    }
}