#ifndef FILESYSTEM_COMMAND_H
#define FILESYSTEM_COMMAND_H

#include "structHeader.h"

void startCommand(string command);
void commandFactory(int key, string message);
bool isFileSystemLoad();

#endif