//
// Created by anton on 12.11.2020.
//

#ifndef OS4_PHMEMORY_H
#define OS4_PHMEMORY_H

#include <vector>
#include "Block.h"


class PhMemory {

private:

    std::vector<Block*> blocks;
    int size_of_memory;
    int allocated_memory;
public:

    PhMemory(int size);

    int getSizeOfMemory() const;

    bool allocateValue(int idx, char v);

    Block* getValueAt(int idx)const ;

    int getAllocatedMemory() const;

};


#endif //OS4_PHMEMORY_H
