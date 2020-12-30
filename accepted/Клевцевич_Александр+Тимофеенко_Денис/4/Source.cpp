#include "FileSystem.h"
#include <iostream>

int main() {
	setlocale(LC_ALL, "rus");
	FileSystem myFiles;
	myFiles.createFile("a");
	myFiles.createFile("b");
	myFiles.createFile("c");
	std::cout << myFiles.readFile("b") << std::endl;
	myFiles.deleteFile("b");
	std::cout<<myFiles.readFile("b")<< std::endl;
	myFiles.createFile("d");
	return 0;
}