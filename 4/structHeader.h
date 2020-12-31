#ifndef FILESYSTEM_STRUCTHEADER_H
#define FILESYSTEM_STRUCTHEADER_H

#include <iostream>
#include <cstring>
#include <string>
#include "init.h"
#include "load.h"
#include <regex>
#include <fstream>
#include "commands.h"
#include <boost/algorithm/string.hpp>
#include <sys/io.h>
#include <stdio.h>
#include <fcntl.h>
#include "commandFuncImpl.h"

using namespace std;

const int MAX_FILENAME_LENGTH = 500;
const int BLOCK_SIZE = 508;

const string FILE_NAME_REGEXP = "\".{1,150}?\"";
const string DATA_INP_REGEXP = "(^exit$)|(^ls$)|(^[\\w]+?\\s\".+?\"\\s\".+?\"$)|(^[\\w]+?\\s\".+?\"\\s\\d+$)|(^[\\w]+?\\s\".+?\"$)";
const string NUMBER_REGEXP = "\\d+";
const string DATA = "\".+\"";

#define EMPTY_FILE -2
#define LAST_BLOCK -1
#define FREE_BLOCK BLOCK_SIZE

struct filesystem {
    string fileSystemName = "";
};

struct descriptor {
    char filename[MAX_FILENAME_LENGTH];
    int firstBlockIndex = EMPTY_FILE;
    bool isFree = true;
};

struct block {
    int blockIndex;
    int nextBlockIndex = LAST_BLOCK;
    char value[BLOCK_SIZE];
    int freeSpace = FREE_BLOCK;
};

#endif
