//
// Created by shund on 25.11.2017.
//

#ifndef FILESYSTEM_DATABLOCK_H
#define FILESYSTEM_DATABLOCK_H


#include <cstddef>

class DataBlock {
private:
    unsigned char *blockPTR;

public:
    const static size_t BLOCK_SIZE = 12 * sizeof(char);
    const static size_t POINTER_SIZE = sizeof(unsigned int);

    void write(const char *buffer);

    char *read();

    unsigned int getNext();

    void setNext(unsigned int nextBlockIndex);

    unsigned char *getBlockPTR() const;

    void setBlockPTR(unsigned char *blockPTR);

    bool isEmpty();

};


#endif //FILESYSTEM_DATABLOCK_H
