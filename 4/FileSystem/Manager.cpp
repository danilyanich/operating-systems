
#include <iostream>
#include <conio.h>
#include <string>
#include "Manager.h"

using namespace std;

bool Manager::isFileExists(string name)
{
	for (File &file : files) {
		if (name == file.getName()) return true;
	}
	return false;
}

void Manager::createUniqueName(string &name)
{
	while (isFileExists(name)) {
		name += "1";
	}
}

void Manager::createFile(string name)
{
	if (name == "") return;
	if (isFileExists(name)) {
		createUniqueName(name);
	}
	
	File file(name);
	files.push_back(file);
}

void Manager::deleteFile(string name)
{
	for (int i = 0; files.size(); i++) {
		File file = files[i];
		if (file.getName() == name) {
			file.clearMemory();
			files.erase(files.begin() + i);
			return;
		}
	}
}

void Manager::copyFile(string name)
{
	File newFile;
	for (File &file : files) {
		if (file.getName() == name) {
			newFile = file;
			newFile.setName(name + "_copy");
			files.push_back(newFile);
			return;
		}
	}

}

string Manager::readFile(string name)
{
	for (File &file : files) {
		if (file.getName() == name) {
			return file.getContent();
		}
	}
	return "";
}

void Manager::writeFile(string name, string content)
{
	for (File &file : files) {
		if (file.getName() == name) {
			file.setContent(content);
			return;
		}
	}
}

void Manager::printAllFiles()
{
	for (File &file : files) {
		std::cout << file.getName() << endl;
	}
}
