#pragma once
#include <stdbool.h>
#include <string>
#include <vector>

using namespace std;

struct block {
    string data;
    int count;
    bool isFilled;
}; // block of a memory

struct file {
    string name;
    string extension;
    string data;
    vector<int> blocks;
}; // file in memory

struct memory {
    vector<block*> dump;
    vector<file*> files;
}; // blocked memory

void initfs();
file* touch(string name);
void copy(string name);
void write(string filename, string data);
void read(string filename);
bool checkExist(string name, string extension);
void delete_file(string filename);
void ls();
void rename(string file, string new_name);