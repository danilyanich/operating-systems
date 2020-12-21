#pragma once

#include <iostream>

#include "Block.h"

using namespace std;


class File {
private:
	string name;
	Block *first;
	//string content;
	int blocks;
public:
	File();
	File(string);
	void setName(string);
	string getName();
	void setContent(string);
	string getContent();
	void clearMemory();

	File & operator=(const File& right);
};