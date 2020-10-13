//
// Created by Petri on 10/13/2020.
//

#ifndef FILESYSTEM_INDEXESTABLE_H
#define FILESYSTEM_INDEXESTABLE_H

#include "PhysicalMemory.h"
#include <vector>
#include <string>

struct Index{
    int idx;
    Index* next;
    Index* prev;
};

using namespace std;

class IndexesTable {
public:
    PhysicalMemory memory;
    vector<Index*> indexes;
    explicit IndexesTable(PhysicalMemory memory);
    Index* allocateFile();
    bool appendValueTo(Index* first, char v);
    Index *findFreeIdx();
    static Index *findLastIdxOf(Index* idx);
    string readFile(Index* initial) const ;
    Index *copyFile(Index* idx);
    static void deleteFile(Index* idx);
};


#endif //FILESYSTEM_INDEXESTABLE_H
