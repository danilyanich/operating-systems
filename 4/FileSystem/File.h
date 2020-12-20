#pragma once

#include <iostream>

using namespace std;


class File {
private:
	string name;
	string content;
	int size;
public:
	File();
	File(string);
	File(string, string);
	void setName(string);
	string getName();
	void setContent(string);
	string getContent();
	int getSize();

	File & operator=(const File& right);
};