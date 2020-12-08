#pragma once
#include <string>
class File
{
private :
	std::string fileName;
	std::string content;
	int size;
public :
	File();
	File(std::string, std::string);
	void setFileName(std::string);
	std::string getFileName();
	void setContent(std::string);
	std::string getContent();
	int getSize();
};

