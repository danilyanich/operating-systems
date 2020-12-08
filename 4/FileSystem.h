//
// Created by shund on 25.11.2017.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include <windows.h>
#include <sstream>
#include "DataBlock.h"

class FileSystem {
private:
    static const unsigned int FILE_NAME_SIZE = 8;
    const char *FILE_SYSTEM_NAME = "file_system";
    HANDLE hFile;
    HANDLE hFileMapping;
    size_t fileSize;
    unsigned char *fileMappingPTR;

    DataBlock *fileSystemData;
    unsigned int blockAmount;

    void setFileMemory();
    HANDLE openPhysicalFile(const char *fileName);
    HANDLE createPhysicalFile(const char *fileName);
    HANDLE createFileMapping();
    HANDLE getFileMappingPTR();

    void setFileSystemData();
    unsigned int getBlockAmount();

    void writeBlockData(unsigned int blockIndex, unsigned int lastRecordedBlock, unsigned int recordedBlock, const char *inputBuffer);
    void writeFileData(unsigned int blockIndex, unsigned int lastRecordedBlock, unsigned int fileIndex, const char *fileName);

    int writeFileBlocks(unsigned int firstBlockIndex, const char *inputBuffer);
    std::string readFileBlocks(unsigned int start_block_index);
    int findFileIndex(const char *fileName);

    unsigned int findLastRecordedBlock(unsigned int firstBlock);
    int findEmptyBlock();
    int deleteFileData(unsigned int fileIndex);

public:

    int open(const char *fileSystemName, size_t fileSize=8000);
    int initialize(size_t preferredSize);
    int createFile(const char *fileName);
    std::string read(const char *fileName);
    int write(const char *fileName, const char *inputBuffer);
    int copy(const char *fileFrom, const char *fileTo);
    int renameFile(const char *oldFileName, const char *newFileName);
    int deleteFile(const char *fileName);
    void showAllFiles();
    void dump();

    ~FileSystem();
};


#endif //FILESYSTEM_FILESYSTEM_H
