#pragma once

#include <iostream>
#include <vector>

#include "File.h"



class Manager{
private:
	vector<File> files;
	bool isFileExists(string);
	void createUniqueName(string&);
public:
	void createFile(string);
	void deleteFile(string);
	void copyFile(string);
	string readFile(string);
	void writeFile(string, string);
	void printAllFiles();

};
