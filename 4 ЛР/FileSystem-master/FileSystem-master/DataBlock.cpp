#include <wchar.h>
#include <sstream>
#include <iostream>
#include "DataBlock.h"

void DataBlock::write(const char *buffer) {

    memcpy(blockPTR + POINTER_SIZE, buffer, BLOCK_SIZE);
}

char *DataBlock::read() {

    char *buffer = new char[BLOCK_SIZE];
    memcpy(buffer, blockPTR + POINTER_SIZE, BLOCK_SIZE);
    buffer[BLOCK_SIZE] = '\0';
    return buffer;
}

void DataBlock::setNext(unsigned int nextBlockIndex) {

    memcpy(blockPTR, &nextBlockIndex, POINTER_SIZE);
}

unsigned int DataBlock::getNext() {

    unsigned int *nextBlockIndex = new unsigned int[POINTER_SIZE];
    memcpy(nextBlockIndex, blockPTR, POINTER_SIZE);
    return *nextBlockIndex;
}

bool DataBlock::isEmpty() {

    char *buffer = new char[BLOCK_SIZE];
    memcpy(buffer, blockPTR + POINTER_SIZE, BLOCK_SIZE);
    std::string tmp = buffer;
    return tmp == "";
}

unsigned char *DataBlock::getBlockPTR() const {

    return blockPTR;
}

void DataBlock::setBlockPTR(unsigned char *blockPTR) {

    DataBlock::blockPTR = blockPTR;
}





