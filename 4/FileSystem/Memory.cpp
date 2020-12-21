#include "Memory.h"

vector<Block> Memory::blocks = {};

char Memory::getValueAt(int index) {
	return blocks[index].getContent();
}

void Memory::clearValueAt(int index)
{
	blocks[index].setContent('\0');
}
