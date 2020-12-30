#include <cmath>
#include <iostream>
#include "FileSystem.h"
#include "ErrorsCode.h"

int FileSystem::open(const char *fileSystemName, size_t fileSize) {

    UnmapViewOfFile(this->fileMappingPTR);
    CloseHandle(this->hFileMapping);
    CloseHandle(this->hFile);

    this->fileSize = fileSize;
    this->blockAmount = getBlockAmount();
    this->fileSize = blockAmount * (DataBlock::POINTER_SIZE + DataBlock::BLOCK_SIZE);

    this->hFile = openPhysicalFile(fileSystemName);
    if (this->hFile == INVALID_HANDLE_VALUE) {
        this->hFile = createPhysicalFile(fileSystemName);
        if (this->hFile == INVALID_HANDLE_VALUE) {
            return ErrorsCode::CREATE_FILE_ERROR;
        }
    }

    this->hFileMapping = createFileMapping();
    if (this->hFileMapping == nullptr) {
        CloseHandle(this->hFile);
        return ErrorsCode::CREATE_FILE_MAPPING_ERROR;
    }

    this->fileMappingPTR = static_cast<unsigned char *>(getFileMappingPTR());
    if (this->fileMappingPTR == nullptr) {
        CloseHandle(this->hFileMapping);
        CloseHandle(this->hFile);
        return ErrorsCode::MAP_VIEW_OF_FILE_ERROR;
    }

    setFileSystemData();
    return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;

}

int FileSystem::initialize(size_t fileSize) {
    UnmapViewOfFile(this->fileMappingPTR);
    CloseHandle(this->hFileMapping);
    CloseHandle(this->hFile);

    this->fileSize = fileSize;
    this->blockAmount = getBlockAmount();
    this->fileSize = blockAmount * (DataBlock::POINTER_SIZE + DataBlock::BLOCK_SIZE);

    this->hFile = createPhysicalFile(FileSystem::FILE_SYSTEM_NAME);
    if (this->hFile == INVALID_HANDLE_VALUE) {
        return ErrorsCode::CREATE_FILE_ERROR;
    }

    setFileMemory();

    this->hFileMapping = createFileMapping();
    if (this->hFileMapping == nullptr) {
        CloseHandle(this->hFile);
        return ErrorsCode::CREATE_FILE_MAPPING_ERROR;
    }

    this->fileMappingPTR = static_cast<unsigned char *>(getFileMappingPTR());
    if (this->fileMappingPTR == nullptr) {
        CloseHandle(this->hFileMapping);
        CloseHandle(this->hFile);
        return ErrorsCode::MAP_VIEW_OF_FILE_ERROR;
    }

    setFileSystemData();
    return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
}

int FileSystem::createFile(const char *fileName) {

    int fileBlockIndex = findEmptyBlock();
    int fileIndex = findFileIndex(fileName);
    if (fileIndex == ErrorsCode::WRONG_FILE_NAME) {

        return ErrorsCode::WRONG_FILE_NAME;
    }  else if (fileIndex != ErrorsCode::FILE_NOT_FOUND) {

        return ErrorsCode::FILE_ALREADY_EXIST;
    } else if (fileBlockIndex != ErrorsCode::LACK_OF_MEMORY) {

        int fileNameBlock = 0;
        int lastRecordedFile = findLastRecordedBlock(fileNameBlock);
        bool findEmptyBlock = false;
        int blockIndex = 0;

        while (blockIndex != blockAmount && !findEmptyBlock) {
            if (fileSystemData[blockIndex].isEmpty() && blockIndex != fileBlockIndex) {
                writeFileData(blockIndex, lastRecordedFile, fileBlockIndex, fileName);
                findEmptyBlock = true;
            }
            blockIndex++;
        }
        if (findEmptyBlock) {
            return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
        } else {
            return ErrorsCode::LACK_OF_MEMORY;
        }
    } else {
        return ErrorsCode::LACK_OF_MEMORY;
    }
}

void FileSystem::setFileMemory() {

    char chunk[64];
    memset(chunk, 0, this->fileSize);
    WriteFile(hFile, chunk, this->fileSize, nullptr, nullptr);
}

HANDLE FileSystem::createPhysicalFile(const char *fileName) {

    return CreateFile(fileName,
                      GENERIC_READ | GENERIC_WRITE,
                      0,
                      nullptr,
                      CREATE_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL,
                      nullptr
    );
}

HANDLE FileSystem::openPhysicalFile(const char *fileName) {

    return CreateFile(fileName,
                      GENERIC_READ | GENERIC_WRITE,
                      0,
                      nullptr,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      nullptr
    );
}

HANDLE FileSystem::createFileMapping() {

    return CreateFileMapping(this->hFile,
                             nullptr,
                             PAGE_READWRITE,
                             0,
                             this->fileSize,
                             nullptr
    );
}

HANDLE FileSystem::getFileMappingPTR() {

    return MapViewOfFile(this->hFileMapping,
                         FILE_MAP_WRITE,
                         0,
                         0,
                         this->fileSize
    );
}

void FileSystem::setFileSystemData() {

    unsigned int blockAmount = getBlockAmount();
    this->fileSystemData = new DataBlock[blockAmount];
    for (int blockIndex = 0; blockIndex < blockAmount; blockIndex++) {
        fileSystemData[blockIndex].setBlockPTR(fileMappingPTR + blockIndex * (DataBlock::POINTER_SIZE + DataBlock::BLOCK_SIZE));
    }
    fileSystemData[0].write("^init^");
}

unsigned int FileSystem::getBlockAmount() {

    return this->fileSize / (DataBlock::POINTER_SIZE + DataBlock::BLOCK_SIZE);
}

int FileSystem::writeFileBlocks(unsigned int firstBlockIndex, const char *inputBuffer) {

    unsigned int blockAmount = getBlockAmount();
    if (firstBlockIndex < blockAmount) {
        unsigned int recordedBlocks = ceil( (double) strlen(inputBuffer) / (double) DataBlock::BLOCK_SIZE);
        unsigned int lastRecordedBlock = findLastRecordedBlock(firstBlockIndex);
//        char* lastRecord = fileSystemData[lastRecordedBlock].read();
//        int offset = strlen(lastRecord);
//        memcpy(fileSystemData[lastRecordedBlock].getBlockPTR()+offset+DataBlock::POINTER_SIZE,inputBuffer,
//        DataBlock::BLOCK_SIZE-offset);
//        char *buffer = new char[strlen(inputBuffer) - (DataBlock::BLOCK_SIZE-offset)];
//        memcpy(buffer,inputBuffer+(DataBlock::BLOCK_SIZE-offset),
//               strlen(inputBuffer) - (DataBlock::BLOCK_SIZE-offset));
        bool findEmptyBlock;
        for (int recordedBlockIndex = 0; recordedBlockIndex < recordedBlocks; recordedBlockIndex++) {
            unsigned int blockIndex = 0;
            findEmptyBlock = false;
            while (blockIndex != blockAmount && !findEmptyBlock) {
                if (fileSystemData[blockIndex].isEmpty()) {
                    writeBlockData(blockIndex, lastRecordedBlock, recordedBlockIndex, inputBuffer);
                    lastRecordedBlock = blockIndex;
                    findEmptyBlock = true;
                }
                blockIndex++;
            }
        }
        if (findEmptyBlock) {
            return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
        } else {
            return ErrorsCode::LACK_OF_MEMORY;
        }
    } else {
        return ErrorsCode::LACK_OF_MEMORY;
    }
}

std::string FileSystem::read(const char *fileName) {

    int fileIndex = findFileIndex(fileName);
    if (fileIndex == ErrorsCode::FILE_NOT_FOUND) {
        return "[ERROR] FILE NOT FOUND\a";
    } else if (fileIndex == ErrorsCode::WRONG_FILE_NAME) {
        return "[ERROR] WRONG FILE NAME\a";
    } else {
        std::string str = readFileBlocks(fileIndex);
        if(str == "") str = "";
        return str;
    }
}

int FileSystem::write(const char *fileName, const char *inputBuffer) {

    int fileIndex = findFileIndex(fileName);
    if (fileIndex == ErrorsCode::WRONG_FILE_NAME) {
        return ErrorsCode::WRONG_FILE_NAME;
    } else if (fileIndex == ErrorsCode::FILE_NOT_FOUND) {
        return ErrorsCode::FILE_NOT_FOUND;
    } else {
        return writeFileBlocks(fileIndex, inputBuffer);
    }
}

int FileSystem::copy(const char *fileFrom, const char *fileTo) {

    int fileFromIndex = findFileIndex(fileFrom);
    int fileToIndex = findFileIndex(fileTo);
    if (fileFromIndex == ErrorsCode::WRONG_FILE_NAME) {

        return ErrorsCode::WRONG_FILE_NAME;
    } else if (fileFromIndex == ErrorsCode::FILE_NOT_FOUND) {

        return ErrorsCode::FILE_NOT_FOUND;
    } else if (fileToIndex == ErrorsCode::WRONG_FILE_NAME) {

        return ErrorsCode::WRONG_FILE_NAME;
    } else if (fileToIndex != ErrorsCode::FILE_NOT_FOUND) {

        return ErrorsCode::FILE_ALREADY_EXIST;
    } else {

        createFile(fileTo);
        write(fileTo, readFileBlocks(fileFromIndex).c_str());
        return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
    }
}

int FileSystem::renameFile(const char *oldFileName, const char *newFileName) {

    int oldFileIndex = findFileIndex(oldFileName);
    int newFileIndex = findFileIndex(newFileName);

    if (oldFileIndex == ErrorsCode::WRONG_FILE_NAME) {

        return ErrorsCode::WRONG_FILE_NAME;
    } else if (newFileIndex == ErrorsCode::WRONG_FILE_NAME) {

        return ErrorsCode::WRONG_FILE_NAME;
    } else if (oldFileIndex == ErrorsCode::FILE_NOT_FOUND) {
        return ErrorsCode::FILE_NOT_FOUND;
    } else if (newFileIndex != ErrorsCode::FILE_NOT_FOUND) {

        return ErrorsCode::FILE_ALREADY_EXIST;
    } else {

        char *fileNameBuffer = new char[FILE_NAME_SIZE];
        unsigned int *fileIndexBuffer = new unsigned int[DataBlock::POINTER_SIZE];
        unsigned int fileBlockIndex = 0;
        do {
            fileBlockIndex = fileSystemData[fileBlockIndex].getNext();
            memcpy(fileNameBuffer, fileSystemData[fileBlockIndex].getBlockPTR() + DataBlock::POINTER_SIZE, DataBlock::BLOCK_SIZE - DataBlock::POINTER_SIZE);
            if (strcmp(oldFileName, fileNameBuffer) == 0) {
                memcpy(fileIndexBuffer, fileSystemData[fileBlockIndex].getBlockPTR() + DataBlock::BLOCK_SIZE, DataBlock::POINTER_SIZE);
                fileSystemData[*fileIndexBuffer].write(newFileName);
                memcpy(fileSystemData[fileBlockIndex].getBlockPTR() + DataBlock::POINTER_SIZE, newFileName, DataBlock::BLOCK_SIZE - DataBlock::POINTER_SIZE);
                return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
            }
        }while (fileBlockIndex != 0);
    }
}

int FileSystem::deleteFile(const char *fileName) {

    unsigned int currentFileIndex = findFileIndex(fileName);
    if (currentFileIndex == ErrorsCode::WRONG_FILE_NAME) {

        return ErrorsCode::WRONG_FILE_NAME;
    } else if (currentFileIndex == ErrorsCode::FILE_NOT_FOUND) {

        return ErrorsCode::FILE_NOT_FOUND;
    } else {

        char chunk[DataBlock::BLOCK_SIZE + DataBlock::POINTER_SIZE];
        memset(chunk, 0, DataBlock::BLOCK_SIZE + DataBlock::POINTER_SIZE);
        char *fileNameBuffer = new char[FILE_NAME_SIZE];
        char *fileIndexBuffer = new char[DataBlock::POINTER_SIZE];
        currentFileIndex = 0;
        unsigned int previousIndex;
        do {
            previousIndex = currentFileIndex;
            currentFileIndex = fileSystemData[previousIndex].getNext();

            memcpy(fileNameBuffer, fileSystemData[currentFileIndex].getBlockPTR() + DataBlock::POINTER_SIZE, DataBlock::BLOCK_SIZE - DataBlock::POINTER_SIZE);

            if (strcmp(fileName, fileNameBuffer) == 0) {
                unsigned int nextIndex = fileSystemData[currentFileIndex].getNext();

                memcpy(fileIndexBuffer, fileSystemData[currentFileIndex].getBlockPTR() + DataBlock::BLOCK_SIZE, DataBlock::POINTER_SIZE);

                memcpy(fileSystemData[currentFileIndex].getBlockPTR(), chunk, DataBlock::BLOCK_SIZE + DataBlock::POINTER_SIZE);

                fileSystemData[previousIndex].setNext(nextIndex);
                return deleteFileData(*fileIndexBuffer);
            }

        }while (currentFileIndex != 0);
    }
}

int FileSystem::deleteFileData(unsigned int fileIndex) {

    char *chunk = new char[DataBlock::POINTER_SIZE + DataBlock::BLOCK_SIZE];
    memset(chunk, 0, DataBlock::POINTER_SIZE + DataBlock::BLOCK_SIZE);
    unsigned int nextIndex;
    do {
        nextIndex = fileSystemData[fileIndex].getNext();
        memcpy(fileSystemData[fileIndex].getBlockPTR(), chunk, DataBlock::POINTER_SIZE + DataBlock::BLOCK_SIZE);
        fileIndex = nextIndex;
    }while (fileIndex != 0);
}

void FileSystem::showAllFiles() {

    unsigned int fileIndex = 0;
    unsigned int nextFileIndex;
    char *buffer = new char[DataBlock::BLOCK_SIZE - DataBlock::POINTER_SIZE];
    std::string fileName;
    while(true) {
        fileIndex = fileSystemData[fileIndex].getNext();
        if(fileIndex == 0) break;
        memcpy(buffer, fileSystemData[fileIndex].getBlockPTR() + DataBlock::POINTER_SIZE, DataBlock::BLOCK_SIZE - DataBlock::POINTER_SIZE);
        fileName = buffer;
        std::cout << fileName << "\n";
        nextFileIndex = fileSystemData[fileIndex].getNext();
    };
}

void FileSystem::dump(){
    unsigned int fileIndex = 0;
    unsigned int nextFileIndex;
    char *buffer = new char[DataBlock::BLOCK_SIZE - DataBlock::POINTER_SIZE];
    std::string fileName;
    while(true) {
        fileIndex = fileSystemData[fileIndex].getNext();
        if(fileIndex == 0) break;
        memcpy(buffer, fileSystemData[fileIndex].getBlockPTR() + DataBlock::POINTER_SIZE, DataBlock::BLOCK_SIZE - DataBlock::POINTER_SIZE);
        fileName = buffer;
        std::cout << "file name: " << fileName << "\n";
        std::cout << read(fileName.c_str())<<"\n";
        nextFileIndex = fileSystemData[fileIndex].getNext();
    };
}

std::string FileSystem::readFileBlocks(unsigned int start_block_index) {

    std::string data_buffer;
    DataBlock data_block;
    unsigned int block_index = fileSystemData[start_block_index].getNext();
    while(true) {
        if(block_index == 0) break;
        data_block = fileSystemData[block_index];
        data_buffer += data_block.read();
        block_index = data_block.getNext();
    } ;
    return data_buffer;
}

void FileSystem::writeBlockData(unsigned int blockIndex, unsigned int lastRecordedBlock, unsigned int recordedBlock, const char *inputBuffer) {

    char *buffer = new char[DataBlock::BLOCK_SIZE];
    memcpy(buffer, inputBuffer + recordedBlock * DataBlock::BLOCK_SIZE, DataBlock::BLOCK_SIZE);
    fileSystemData[blockIndex].write(buffer);
    fileSystemData[lastRecordedBlock].setNext(blockIndex);
}

void FileSystem::writeFileData(unsigned int blockIndex, unsigned int lastRecordedBlock, unsigned int fileIndex,
                               const char *fileName) {

    fileSystemData[fileIndex].write(fileName);
    fileSystemData[lastRecordedBlock].setNext(blockIndex);
    memcpy(fileSystemData[blockIndex].getBlockPTR() + DataBlock::POINTER_SIZE, fileName, DataBlock::BLOCK_SIZE - DataBlock::POINTER_SIZE);
    memcpy(fileSystemData[blockIndex].getBlockPTR() + DataBlock::BLOCK_SIZE, &fileIndex, DataBlock::POINTER_SIZE);
}

unsigned int FileSystem::findLastRecordedBlock(unsigned int firstBlock) {

    unsigned int lastRecordedBlock, previousBlock, currentBlock = firstBlock;
    do {
        previousBlock = currentBlock;
        currentBlock = fileSystemData[currentBlock].getNext();
        lastRecordedBlock = previousBlock;
    } while(currentBlock != 0);
    return lastRecordedBlock;
}

int FileSystem::findEmptyBlock() {

    for (int blockIndex = 0; blockIndex < blockAmount; blockIndex++) {
        if (fileSystemData[blockIndex].isEmpty()) {
            return blockIndex;
        }
    }
    return ErrorsCode::LACK_OF_MEMORY;
}

int FileSystem::findFileIndex(const char *fileName) {

    if (strlen(fileName) <= FILE_NAME_SIZE) {
        unsigned int *fileIndex = new unsigned int[DataBlock::POINTER_SIZE];
        char *fileNameTmp = new char[FILE_NAME_SIZE];
        unsigned int fileNameBlock = 0;
        do {
            fileNameBlock = fileSystemData[fileNameBlock].getNext();
            memcpy(fileNameTmp, fileSystemData[fileNameBlock].getBlockPTR() + DataBlock::POINTER_SIZE, DataBlock::BLOCK_SIZE - DataBlock::POINTER_SIZE);
            if (strcmp(fileName, fileNameTmp) == 0) {
                memcpy(fileIndex, fileSystemData[fileNameBlock].getBlockPTR() + DataBlock::BLOCK_SIZE, DataBlock::POINTER_SIZE);
                return *fileIndex;
            }
        }while (fileNameBlock != 0);
        return ErrorsCode::FILE_NOT_FOUND;
    } else {
        return ErrorsCode::WRONG_FILE_NAME;
    }
}


FileSystem::~FileSystem() {
    delete fileSystemData;
    UnmapViewOfFile(this->fileMappingPTR);
    CloseHandle(this->hFileMapping);
    CloseHandle(this->hFile);

}
