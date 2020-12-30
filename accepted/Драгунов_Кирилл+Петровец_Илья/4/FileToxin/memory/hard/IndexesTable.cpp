//
// Created by Petri on 10/13/2020.
//

#include "IndexesTable.h"

Index *IndexesTable::allocateFile() {
    Index *res = findFreeIdx();
    if (res != nullptr) {
        memory.allocateValue(res->idx, '\0');
    }
//    for (auto& i : this->indexes) {
//        printf("%d\n", i->idx);
//    }
    return res;
}

IndexesTable::IndexesTable(PhysicalMemory memory) : memory(memory) {
    for (int i = 0; i < memory.getTotalMemory(); i++) {
        this->indexes.
                push_back(new Index{-1,
                                    nullptr,
                                    nullptr});
    }
}

bool IndexesTable::appendValueTo(Index *first, char v) {
    if (memory.getValueAt(first->idx) == '\0') {
        memory.allocateValue(first->idx, v);
        return true;
    }
    Index *freeIdx = findFreeIdx();
    if (freeIdx != nullptr) {
        memory.allocateValue(freeIdx->idx, v);
        Index *last = findLastIdxOf(first);
        last->next = freeIdx;
        freeIdx->prev = last;
        return true;
    }
    //printf("Here\n");
    return false;
}

Index *IndexesTable::findFreeIdx() {
    for (int i = 0; i < indexes.size(); i++) {
        if (indexes[i]->idx == -1) {
            indexes[i]->idx = i;
            //printf("%d\n", i);
            return indexes[i];
        }
        //printf("Invalid:%d\n", i);
    }
    return nullptr;
}

Index *IndexesTable::findLastIdxOf(Index* idx) {
    if (idx->next == nullptr) {
        return idx;
    } else {
        Index* copy = idx;
        while (copy->next != nullptr) {
            copy = copy->next;
        }
        return copy;
    }
}

std::string IndexesTable::readFile(Index *initial) const {
    Index *copy = initial;
    std::string res;
    do {
        res.insert(res.end(), memory.getValueAt(copy->idx));
        copy = copy->next;
    } while (copy != nullptr);
    return res;
}

Index *IndexesTable::copyFile(Index *idx) {
    std::string res = readFile(idx);
    Index *newFile = allocateFile();
    for (char re : res) {
        appendValueTo(newFile, re);
    }
    return newFile;
}

void IndexesTable::deleteFile(Index *idx) {
    while (idx != nullptr) {
        idx->idx = -1;
        idx->prev = nullptr;
        Index *buf = idx;
        idx = idx->next;
        buf->next = nullptr;
    }
}
