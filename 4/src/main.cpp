#include <iostream>
#include <string>
#include <cassert>
#include "file-system/FileSystem.h"


int main() {
    PhysicalMemory mem(std::string(100, ' '));
    FileSystem fs(mem);
    fs.CreateDir("dir1");
    fs.CreateFile("file1", "body1");
    fs.CreateFile("file2", "body2");
    fs.ChangeDirectory("dir1");
    fs.CreateFile("file3", "body3");
    fs.CreateDir("dir2");
    fs.CreateDir("dir3");
    fs.ChangeDirectory("dir3");
    fs.CreateFile("file4", "body4");

    fs.Dump();
}