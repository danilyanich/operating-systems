#include "PhMemory.h"

int PhMemory::getSizeOfMemory() const {
    return size_of_memory;
}

int PhMemory::getAllocatedMemory() const {
    return allocated_memory;
}

PhMemory::PhMemory(int size) {
    this->blocks.resize(size);

    for (int i = 0; i < blocks.size(); i++) {
        blocks[i] = new Block();

    }
    this->size_of_memory = size;
    this->allocated_memory = 0;
}

bool PhMemory::allocateValue(int idx, char v)
{
    if (allocated_memory == size_of_memory) {
        return false;
    }
    this->blocks[idx]->cont = v;
    this->blocks[idx]->id = idx;
    this->allocated_memory += 1;

    return true;

}

Block* PhMemory::getValueAt(int idx) const {
    return this->blocks[idx];
}