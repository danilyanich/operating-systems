//
// Created by Petri on 10/13/2020.
//

#ifndef FILESYSTEM_PHYSICALMEMORY_H
#define FILESYSTEM_PHYSICALMEMORY_H


class PhysicalMemory {
    char *blocks;
    unsigned int totalMemory;
    int allocated;

public:
    unsigned int getTotalMemory() const;

    int getAllocated() const;

    explicit PhysicalMemory(int size);

    bool allocateValue(int idx, char v) {
        if (allocated == totalMemory) {
            return false;
        }
        this->blocks[idx] = v;
        this->allocated += 1;
        return true;
    }

    char getValueAt(int idx) const {
        return this->blocks[idx];
    }
};


#endif //FILESYSTEM_PHYSICALMEMORY_H
