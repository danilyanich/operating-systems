#ifndef FILESYSTEM_FILECOMMAND_H
#define FILESYSTEM_FILECOMMAND_H

#include "structHeader.h"
#include <iostream>
using namespace std;

void init(string message);
string load(string message, struct filesystem fileSystem);
void create(string message, struct filesystem fileSystem);
void remove(string message, struct filesystem fileSystem);
void copy(string message, struct filesystem fileSystem);
void move(string message, struct filesystem fileSystem);
void write(string message, struct filesystem fileSystem);
void read(string message, struct filesystem fileSystem);
void ls(struct filesystem fileSystem);

#endif