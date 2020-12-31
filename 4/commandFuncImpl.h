#ifndef FILESYSTEM_COMMONFUNC_H
#define FILESYSTEM_COMMONFUNC_H

#include <iostream>
#include "structHeader.h"

using namespace std;

bool isFileExist(string filename, struct filesystem fileSystem);
bool isFreeFileExist(struct filesystem fileSystem);
void removeFunc(string filename, struct filesystem fileSystem);
void clearBlockFunc(int firstBlockIndex, struct filesystem fileSystem);
void writeFunc(string filename, string message, struct filesystem fileSystem);
int findFreeBlock(struct filesystem fileSystem);
void write(string message, int blocIndex, struct filesystem fileSystem);
string readFunc(string filename, struct filesystem fileSystem);
string read(int blocIndex, struct filesystem fileSystem);

#endif