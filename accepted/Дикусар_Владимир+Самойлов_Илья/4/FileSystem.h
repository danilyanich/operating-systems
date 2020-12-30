#pragma once
#include <iostream>
#include <list>
#include "File.h"
class FileSystem
{
private:
	std::list<File> files;
public:
	FileSystem();
	void createFile(std::string);
	void deleteFile(std::string);
	std::string readFile(std::string);
	void writeFile(std::string);
	File& copy(std::string);
};

