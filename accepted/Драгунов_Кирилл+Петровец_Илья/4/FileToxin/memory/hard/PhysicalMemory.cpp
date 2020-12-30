//
// Created by Petri on 10/13/2020.
//

#include "PhysicalMemory.h"

unsigned int PhysicalMemory::getTotalMemory() const {
    return totalMemory;
}

int PhysicalMemory::getAllocated() const {
    return allocated;
}

PhysicalMemory::PhysicalMemory(int size) {
    this->blocks = new char[size];
    this->totalMemory = size;
    this->allocated = 0;
}

