#include "File.h"
#include "Block.h"
#include "Memory.h"

using namespace std;

File::File()
{
	name = "";
}

File::File(string name)
{
	this->name = name;
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
	blocks = content.size();
	first = new Block(content[0]);
	for (int i = 1; i < blocks; i++) {
		new Block(content[i]);
	}

}

string File::getContent()
{
	string retValue;
	int id = first->getId();
	for (int i = id; i < id + blocks; i++) {
		retValue += Memory::getValueAt(i);
	}

	return retValue;
}

void File::clearMemory()
{
	int id = first->getId();
	for (int i = id; i < id + blocks; i++) {
		Memory::clearValueAt(i);
	}
}

File & File::operator=(const File & right)
{
	if (this == &right) {
		return *this;
	}

	name = right.name;
	blocks = right.blocks;
	first = new Block(right.first->getContent());
	for (int i = 1; i < blocks; i++) {
		int index = right.first->getId() + i;
		new Block(Memory::getValueAt(index));
	}

	return *this;
}


