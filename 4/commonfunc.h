#ifndef FILESYSTEM_COMMONFUNC_H
#define FILESYSTEM_COMMONFUNC_H

#include <iostream>
#include "structHeader.h"
using namespace std;

bool isFileExist(string filename, struct filesystem fs);
bool isFreeFileExist(struct filesystem fs);
void removeFunc(string filename, struct filesystem fs);
void clearBlockFunc(int firstBlockIndex, struct filesystem fs);
void writeFunc(string filename, string message, struct filesystem fs);
int findFreeBlock(struct filesystem fs);
void write(string message, int blocIndex, struct filesystem fs);
string readFunc(string filename, struct filesystem fs);
string read(int blocIndex, struct filesystem fs);

#endif