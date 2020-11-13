//
// Created by anton on 12.11.2020.
//

#include "PhMemory.h"

int PhMemory::getSizeOfMemory() const {
    return size_of_memory;
}

int PhMemory::getAllocatedMemory() const {
    return allocated_memory;
}

PhMemory::PhMemory(int size)  {
    this->blocks.resize(size);

    for(int i =0;i<blocks.size()-1;i++){
        blocks[i] =new Block();
        blocks[i+1] =new Block();
        blocks[i]->next=blocks[i+1];
    }
    this->size_of_memory=size;
    this->allocated_memory=0;
}

bool PhMemory::allocateValue(int idx, char v)
{
    if (allocated_memory == size_of_memory) {
        return false;
    }
    this->blocks[idx]->cont=v;
    this->allocated_memory += 1;

    return true;

}

char PhMemory::getValueAt(int idx) {
    return this->blocks[idx]->cont;
}