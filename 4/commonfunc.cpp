#include <boost/algorithm/string/classification.hpp>
#include "commonfunc.h"

bool isFileExist(string filename, struct filesystem fs) {
    bool result = false;
    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = 0;
    while (index < len / 2) {
        block b;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));

        descriptor d;
        memcpy(&d, &b.value, BLOCK_SIZE);

        if (d.isFree == false) {
            if (strcmp(d.filename, filename.c_str()) == 0) {
                result = true;
                break;
            }
        }
        index += sizeof(b);
    }
    close(fd);
    return result;
}
bool isFreeFileExist(struct filesystem fs) {
    bool result = false;
    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = 0;
    while (index < len / 2) {
        block b;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));

        descriptor d;
        memcpy(&d, &b.value, BLOCK_SIZE);

        if (d.isFree == true) {
            result = true;
            break;
        }
        index += sizeof(b);
    }
    close(fd);
    return result;
}

void removeFunc(string filename, struct filesystem fs) {
    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = 0;
    descriptor findDesc;
    block findBlock;
    while (index < len / 2) {
        block b;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));

        descriptor d;
        memcpy(&d, &b.value, BLOCK_SIZE);

        if (d.isFree == false) {
            if (strcmp(d.filename, filename.c_str()) == 0) {
                findDesc = d;
                findBlock = b;
                break;
            }
        }
        index += sizeof(b);
    }

    findDesc.isFree = true;
    if (findDesc.firstBlockIndex == EMPTY_FILE) {
        memcpy(&findBlock.value, &findDesc, BLOCK_SIZE);
        lseek(fd, index, SEEK_SET);
        write(fd, &findBlock, sizeof(findBlock));
        close(fd);
        return;
    } else {
        int firstBlockIndex = findDesc.firstBlockIndex;
        findDesc.firstBlockIndex = EMPTY_FILE;
        memcpy(&findBlock.value, &findDesc, BLOCK_SIZE);
        lseek(fd, index, SEEK_SET);
        write(fd, &findBlock, sizeof(findBlock));
        close(fd);
        clearBlockFunc(firstBlockIndex, fs);
        return;
    }
}
void clearBlockFunc(int firstBlockIndex, struct filesystem fs) {
    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = len / 2;
    while (index < len) {
        block b;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));

        if (b.blockIndex == firstBlockIndex) {
            b.freeSpace = FREE_BLOCK;
            if (b.nextBlockIndex == LAST_BLOCK) {
                lseek(fd, index, SEEK_SET);
                write(fd, &b, sizeof(b));
                close(fd);
                return;
            } else {
                firstBlockIndex = b.nextBlockIndex;
                b.nextBlockIndex = LAST_BLOCK;
                lseek(fd, index, SEEK_SET);
                write(fd, &b, sizeof(b));
                index = len / 2;
                continue;
            }
        }
        index += sizeof(b);
    }
    close(fd);
}
void writeFunc(string filename, string message, struct filesystem fs) {
    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = 0;
    int firstBlockIndex;
    block b;
    descriptor d;

    while (index < len / 2) {
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));
        memcpy(&d, &b.value, BLOCK_SIZE);
        if (d.isFree == false) {
            if (strcmp(d.filename, filename.c_str()) == 0) {
                firstBlockIndex = d.firstBlockIndex;
                break;
            }
        }
        index += sizeof(b);
    }
    close(fd);

    if (firstBlockIndex == EMPTY_FILE) {
        int offset = findFreeBlock(fs);
        if (offset == -1) {
            cout << "filesystem is completed\n";
            return;
        } else {
            fd = open(fileSystemName.c_str(), O_RDWR);
            block firstBlock;
            lseek(fd, offset, SEEK_SET);
            read(fd, &firstBlock, sizeof(firstBlock));
            firstBlockIndex = firstBlock.blockIndex;
            d.firstBlockIndex = firstBlockIndex;
            memcpy(&b.value, &d, BLOCK_SIZE);
            lseek(fd, index, SEEK_SET);
            write(fd, &b, sizeof(b));
            close(fd);
        }
    }
    write(message, firstBlockIndex, fs);
}
int findFreeBlock(struct filesystem fs) {
    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = len / 2;
    while (index < len) {
        block b;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));
        if (b.freeSpace == FREE_BLOCK) {
            close(fd);
            return index;
        }
        index += sizeof(b);
    }
    close(fd);
    return -1;
}
void write(string message, int blocIndex, struct filesystem fs) {
    int mesLen = message.length();
    int remainLen = message.length();

    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = len / 2;
    while (index < len) {
        block b;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));
        if (b.blockIndex == blocIndex) {
            while (b.freeSpace > 0 && remainLen != 0) {
                char ch = message.c_str()[mesLen - remainLen];
                b.value[BLOCK_SIZE - b.freeSpace] = ch;
                remainLen--;
                b.freeSpace--;
                lseek(fd, index, SEEK_SET);
                write(fd, &b, sizeof(b));
            }

            if (remainLen == 0) {
                cout << "written successful\n";
                close(fd);
                return;
            }

            if (b.nextBlockIndex != LAST_BLOCK) {
                blocIndex = b.nextBlockIndex;
                index = len / 2;
                continue;
            } else {
                int offset = findFreeBlock(fs);
                if (offset == -1) {
                    cout << "filesystem is completed: was written\\copied not all data\n";
                    close(fd);
                    return;
                } else {
                    block nextBlock;
                    lseek(fd, offset, SEEK_SET);
                    read(fd, &nextBlock, sizeof(nextBlock));
                    b.nextBlockIndex = nextBlock.blockIndex;
                    lseek(fd, index, SEEK_SET);
                    write(fd, &b, sizeof(b));
                    blocIndex = nextBlock.blockIndex;
                    index = len / 2;
                    continue;
                }

            }
        }
        index += sizeof(b);
    }
}
string readFunc(string filename, struct filesystem fs) {
    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = 0;
    int firstBlockIndex;
    while (index < len / 2) {
        block b;
        descriptor d;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));
        memcpy(&d, &b.value, BLOCK_SIZE);

        if (d.isFree == false) {
            if (strcmp(d.filename, filename.c_str()) == 0) {
                firstBlockIndex = d.firstBlockIndex;
                break;
            }
        }
        index += sizeof(b);
    }
    close(fd);

    if (firstBlockIndex == EMPTY_FILE) {
        string result;
        result.insert(result.length(), filename.c_str());
        result.insert(result.length(), " is empty");
        return result;
    }

    return read(firstBlockIndex, fs);
}
string read(int blocIndex, struct filesystem fs) {
    string result;
    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = len / 2;
    while (index < len) {
        block b;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));
        if (b.blockIndex == blocIndex) {
            result.insert(result.length(), b.value);
            if (b.nextBlockIndex != LAST_BLOCK) {
                blocIndex = b.nextBlockIndex;
                index = len / 2;
                continue;
            } else {
                break;
            }
        }
        index += sizeof(b);
    }
    close(fd);
    return result;
}