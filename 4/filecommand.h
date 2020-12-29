#ifndef FILESYSTEM_FILECOMMAND_H
#define FILESYSTEM_FILECOMMAND_H

#include "structHeader.h"
#include <iostream>
using namespace std;

void create(string message, struct filesystem fs);
void remove(string message, struct filesystem fs);
void copy(string message, struct filesystem fs);
void move(string message, struct filesystem fs);
void write(string message, struct filesystem fs);
void read(string message, struct filesystem fs);
void ls(struct filesystem fs);

#endif