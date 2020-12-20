#include "File.h"

using namespace std;

File::File()
{
	name = "";
	content = "";
	size = 0;
}

File::File(string name)
{
	this->name = name;
	size = 0;
}

File::File(string name, string content)
{
	this->name = name;
	this->content = content;
	this->size = content.size();
}

void File::setName(string name)
{
	this->name = name;
}

string File::getName()
{
	return name;
}

void File::setContent(string content)
{
	this->content = content;
	this->size = content.size();
}

string File::getContent()
{
	return content;
}

int File::getSize()
{
	return size;
}

File & File::operator=(const File & right)
{
	if (this == &right) {
		return *this;
	}

	name = right.name;
	content = right.content;
	size = right.size;
	return *this;
}


